#pragma once
#include "lua_conversion/lua_conversion.hpp"
#include "LuaBasicConversions.h"
#include "L2DModel.h"
#include <cstring>

// Keep the old module's checked conversions in Release as well as Debug.
// Vec2/Color4B accept the legacy partial tables; integers also accept numeric
// strings. This policy is local to Live2D and does not alter other bindings.
namespace live2d_binding
{
	template<typename T>
	bool read(lua_State* L, int index, T* out, const char* name)
	{
		if constexpr (std::is_same_v<T, bool>)
			return luaval_to_boolean(L, index, out, name);
		else if constexpr (std::is_floating_point_v<T>) {
			double value;
			const bool ok = luaval_to_number(L, index, &value, name);
			if (ok) *out = static_cast<T>(value);
			return ok;
		} else if constexpr (std::is_same_v<T, int32_t>)
			return luaval_to_int32(L, index, out, name);
		else if constexpr (std::is_same_v<T, uint8_t>) {
			uint16_t value;
			const bool ok = luaval_to_uint16(L, index, &value, name);
			if (ok) *out = static_cast<T>(value);
			return ok;
		} else if constexpr (std::is_same_v<T, std::string>)
			return luaval_to_std_string(L, index, out, name);
		else if constexpr (std::is_same_v<T, cocos2d::Vec2>)
			return luaval_to_vec2(L, index, out, name);
		else if constexpr (std::is_same_v<T, cocos2d::Color4B>)
			return luaval_to_color4b(L, index, out, name);
		else if constexpr (std::is_same_v<T, LAppModel::SoundEventHandler>) {
			// The old Lua API accepts functions only; nil is not cancellation.
			return lua_isfunction(L, index) && lua::luaval_to_owned_callback(L, index, out, name);
		} else
			return lua::luaval_to_native(L, index, out, name);
	}

	template<int Index, typename... Args> struct arguments;
	template<int Index, typename A, typename... Args>
	struct arguments<Index, A, Args...> {
		template<typename F, typename... Values>
		static bool call(lua_State* L, F& function, int& current, int& results,
			const char* name, Values&&... values) {
			using value_t = std::remove_cv_t<std::remove_reference_t<A>>;
			value_t value{};
			current = Index;
			if (!read(L, Index, &value, name)) return false;
			return arguments<Index + 1, Args...>::call(L, function, current, results,
				name, std::forward<Values>(values)..., std::forward<A>(value));
		}
	};
	template<int Index> struct arguments<Index> {
		template<typename F, typename... Values>
		static bool call(lua_State* L, F& function, int&, int& results,
			const char* name, Values&&... values) {
			const int top = lua_gettop(L);
			if constexpr (std::is_void_v<std::invoke_result_t<F&, Values...>>)
				std::invoke(function, std::forward<Values>(values)...);
			else {
				decltype(auto) result = std::invoke(function, std::forward<Values>(values)...);
				// Size aliases Vec2 in C++; the public return keeps width/height.
				if constexpr (std::is_same_v<std::decay_t<decltype(result)>, cocos2d::Vec2>) {
					if (std::strcmp(name, "l2d.Model:getCanvasSize") == 0)
						size_to_luaval(L, result);
					else vec2_to_luaval(L, result);
				} else lua::native_to_luaval(L, result);
			}
			results = lua_gettop(L) - top;
			return true;
		}
	};

	template<typename T> struct invocation;
	template<typename R, typename... Args> struct invocation<R(*)(Args...)> {
		template<typename F>
		static bool call(lua_State* L, F& f, int& current, int& results, const char* name) {
			return arguments<2, Args...>::call(L, f, current, results, name);
		}
	};
	template<typename C, typename R, typename... Args>
	struct invocation<R(C::*)(Args...)> {
		template<typename F>
		static bool call(lua_State* L, F& f, int& current, int& results, const char* name) {
			return arguments<1, C*, Args...>::call(L, f, current, results, name);
		}
	};
	template<typename C, typename R, typename... Args>
	struct invocation<R(C::*)(Args...) const> {
		template<typename F>
		static bool call(lua_State* L, F& f, int& current, int& results, const char* name) {
			return arguments<1, const C*, Args...>::call(L, f, current, results, name);
		}
	};
	template<typename T> struct lambda_invocation;
	template<typename R, typename... Args> struct lambda_invocation<R(*)(Args...)> {
		template<bool Member, typename F>
		static bool call(lua_State* L, F& f, int& current, int& results, const char* name) {
			return arguments<Member ? 1 : 2, Args...>::call(L, f, current, results, name);
		}
	};
	template<bool Member, typename F>
	bool call(lua_State* L, F f, int& current, int& results, const char* name) {
		if constexpr (std::is_class_v<F>) {
			// Default-argument lambdas carry self explicitly as their first argument.
			using pointer_t = typename lua::function_traits<F>::pointer;
			return lambda_invocation<pointer_t>::template call<Member>(L, f, current, results, name);
		} else
			return invocation<F>::call(L, f, current, results, name);
	}
}

#ifdef LUA_DEBUG
#define LIVE2D_CHECK_TYPE(L, TYPE, NAME) \
	if (!tolua_isusertype((L), 1, (TYPE), 0, nullptr)) \
		return luaL_error((L), "invalid 'cobj' in '%s': '%s', expects '%s'", \
			(NAME), tolua_typename((L), 1), (TYPE));
#define LIVE2D_CHECK_OBJECT(L, OBJECT, NAME) \
	if (!(OBJECT)) return luaL_error((L), "invalid 'cobj' in '%s'", (NAME));
#else
#define LIVE2D_CHECK_TYPE(L, TYPE, NAME) (void)(TYPE);
#define LIVE2D_CHECK_OBJECT(L, OBJECT, NAME) (void)(OBJECT);
#endif

#undef LUA_INVOKE_HEADER
#define LUA_INVOKE_HEADER(CName_, FName_) \
	constexpr auto LUA_OBJ_TYPE = CName_; \
	constexpr auto LUA_FNAME = FName_; \
	LIVE2D_CHECK_TYPE(lua_S, LUA_OBJ_TYPE, LUA_FNAME); \
	const auto* live2d_self = tolua_tousertype(lua_S, 1, nullptr); \
	LIVE2D_CHECK_OBJECT(lua_S, live2d_self, LUA_FNAME); \
	const int argc = lua_gettop(lua_S) - 1; \
	constexpr bool binding_member_self = true; \
	int arg_cur = -1; \
	bool ok = false;

#undef LUA_SINVOKE_HEADER
#define LUA_SINVOKE_HEADER(CName_, FName_) \
	constexpr auto LUA_OBJ_TYPE = CName_; \
	constexpr auto LUA_FNAME = FName_; \
	LIVE2D_CHECK_TYPE(lua_S, LUA_OBJ_TYPE, LUA_FNAME); \
	const int argc = lua_gettop(lua_S) - 1; \
	constexpr bool binding_member_self = false; \
	int arg_cur = -1; \
	bool ok = false;

#undef LUA_TRY_INVOKE
#define LUA_TRY_INVOKE(ARGC_, Fn_) \
	if (argc == (ARGC_)) { \
		int native_results = 0; \
		ok = live2d_binding::call<binding_member_self>(lua_S, (Fn_), arg_cur, native_results, LUA_FNAME); \
		if (ok) { lua_settop(lua_S, 1); return 1; } \
	}

#undef LUA_TRY_INVOKE_R
#define LUA_TRY_INVOKE_R(ARGC_, Fn_) \
	if (argc == (ARGC_)) { \
		int native_results = 0; \
		ok = live2d_binding::call<binding_member_self>(lua_S, (Fn_), arg_cur, native_results, LUA_FNAME); \
		if (ok) return native_results; \
	}

#undef LUA_INVOKE_FOOTER
#define LUA_INVOKE_FOOTER(Expected_) \
	if (arg_cur != -1) return luaL_error(lua_S, "invalid arguments in '%s'", LUA_FNAME); \
	return luaL_error(lua_S, "wrong number of arguments in '%s': %d, expects %s", LUA_FNAME, argc, \
		std::strcmp(Expected_, "2,3") == 0 ? "2 to 3" : Expected_);
#undef LUA_SINVOKE_FOOTER
#define LUA_SINVOKE_FOOTER(Expected_) LUA_INVOKE_FOOTER(Expected_)
