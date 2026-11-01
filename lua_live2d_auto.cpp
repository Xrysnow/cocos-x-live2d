#include "lua_live2d_auto.hpp"
#include "lua_live2d_compat.hpp"
#include "L2DModel.h"

int lua_l2d_Model_addParameter(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:addParameter");
	LUA_TRY_INVOKE(3, static_cast<void(l2d::Model::*)(const std::string&, float, float)>(&l2d::Model::addParameter));
	LUA_TRY_INVOKE(2, [](l2d::Model* obj,const std::string& arg0,float arg1){{return obj->addParameter(arg0,std::move(arg1));}});
	LUA_INVOKE_FOOTER("2,3");
}
int lua_l2d_Model_areaHitTest(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:areaHitTest");
	LUA_TRY_INVOKE_R(3, static_cast<bool(l2d::Model::*)(const char*, float, float)>(&l2d::Model::areaHitTest));
	LUA_INVOKE_FOOTER("3");
}
int lua_l2d_Model_getCanvasOrigin(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getCanvasOrigin");
	LUA_TRY_INVOKE_R(0, static_cast<cocos2d::Vec2(l2d::Model::*)()const>(&l2d::Model::getCanvasOrigin));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getCanvasSize(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getCanvasSize");
	LUA_TRY_INVOKE_R(0, static_cast<cocos2d::Size(l2d::Model::*)()const>(&l2d::Model::getCanvasSize));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getDebugRectRenderer(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDebugRectRenderer");
	LUA_TRY_INVOKE_R(0, static_cast<cocos2d::DrawNode*(l2d::Model::*)()const>(&l2d::Model::getDebugRectRenderer));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getDirectory(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDirectory");
	LUA_TRY_INVOKE_R(0, static_cast<std::string(l2d::Model::*)()const>(&l2d::Model::getDirectory));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getDrawableAlphaBlendMode(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDrawableAlphaBlendMode");
	LUA_TRY_INVOKE_R(1, static_cast<int32_t(l2d::Model::*)(const std::string&)const>(&l2d::Model::getDrawableAlphaBlendMode));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getDrawableBlendMode(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDrawableBlendMode");
	LUA_TRY_INVOKE_R(1, static_cast<int32_t(l2d::Model::*)(const std::string&)const>(&l2d::Model::getDrawableBlendMode));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getDrawableCount(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDrawableCount");
	LUA_TRY_INVOKE_R(0, static_cast<int32_t(l2d::Model::*)()const>(&l2d::Model::getDrawableCount));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getDrawableCulling(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDrawableCulling");
	LUA_TRY_INVOKE_R(1, static_cast<int32_t(l2d::Model::*)(const std::string&)const>(&l2d::Model::getDrawableCulling));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getDrawableNames(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDrawableNames");
	LUA_TRY_INVOKE_R(0, static_cast<std::vector<std::string>(l2d::Model::*)()const>(&l2d::Model::getDrawableNames));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getDrawableOpacity(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getDrawableOpacity");
	LUA_TRY_INVOKE_R(1, static_cast<float(l2d::Model::*)(const std::string&)const>(&l2d::Model::getDrawableOpacity));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getExpressionNames(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getExpressionNames");
	LUA_TRY_INVOKE_R(0, static_cast<std::vector<std::string>(l2d::Model::*)()const>(&l2d::Model::getExpressionNames));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getGravity(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getGravity");
	LUA_TRY_INVOKE_R(0, static_cast<cocos2d::Vec2(l2d::Model::*)()const>(&l2d::Model::getGravity));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getLipSync(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getLipSync");
	LUA_TRY_INVOKE_R(0, static_cast<bool(l2d::Model::*)()const>(&l2d::Model::getLipSync));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getLipValue(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getLipValue");
	LUA_TRY_INVOKE_R(0, static_cast<float(l2d::Model::*)()const>(&l2d::Model::getLipValue));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getMocVersion(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getMocVersion");
	LUA_TRY_INVOKE_R(0, static_cast<uint32_t(l2d::Model::*)()const>(&l2d::Model::getMocVersion));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getModelColor(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getModelColor");
	LUA_TRY_INVOKE_R(0, static_cast<cocos2d::Color4B(l2d::Model::*)()const>(&l2d::Model::getModelColor));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getModelOpacity(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getModelOpacity");
	LUA_TRY_INVOKE_R(0, static_cast<uint8_t(l2d::Model::*)()const>(&l2d::Model::getModelOpacity));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getMotionCount(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getMotionCount");
	LUA_TRY_INVOKE_R(1, static_cast<size_t(l2d::Model::*)(const std::string&)>(&l2d::Model::getMotionCount));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getMotionGroupNames(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getMotionGroupNames");
	LUA_TRY_INVOKE_R(0, static_cast<std::vector<std::string>(l2d::Model::*)()const>(&l2d::Model::getMotionGroupNames));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getParameter(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getParameter");
	LUA_TRY_INVOKE_R(1, static_cast<float(l2d::Model::*)(const std::string&)const>(&l2d::Model::getParameter));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getParameterCount(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getParameterCount");
	LUA_TRY_INVOKE_R(0, static_cast<int32_t(l2d::Model::*)()const>(&l2d::Model::getParameterCount));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getParameterDefault(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getParameterDefault");
	LUA_TRY_INVOKE_R(1, static_cast<float(l2d::Model::*)(const std::string&)const>(&l2d::Model::getParameterDefault));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getParameterMax(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getParameterMax");
	LUA_TRY_INVOKE_R(1, static_cast<float(l2d::Model::*)(const std::string&)const>(&l2d::Model::getParameterMax));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getParameterMin(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getParameterMin");
	LUA_TRY_INVOKE_R(1, static_cast<float(l2d::Model::*)(const std::string&)const>(&l2d::Model::getParameterMin));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getParameterNames(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getParameterNames");
	LUA_TRY_INVOKE_R(0, static_cast<std::vector<std::string>(l2d::Model::*)()const>(&l2d::Model::getParameterNames));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getPartCount(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getPartCount");
	LUA_TRY_INVOKE_R(0, static_cast<int32_t(l2d::Model::*)()const>(&l2d::Model::getPartCount));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getPartNames(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getPartNames");
	LUA_TRY_INVOKE_R(0, static_cast<std::vector<std::string>(l2d::Model::*)()const>(&l2d::Model::getPartNames));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getPartOpacity(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getPartOpacity");
	LUA_TRY_INVOKE_R(1, static_cast<float(l2d::Model::*)(const std::string&)const>(&l2d::Model::getPartOpacity));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_getPartParents(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getPartParents");
	LUA_TRY_INVOKE_R(0, static_cast<std::vector<int>(l2d::Model::*)()const>(&l2d::Model::getPartParents));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getPixelsPerUnit(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getPixelsPerUnit");
	LUA_TRY_INVOKE_R(0, static_cast<float(l2d::Model::*)()const>(&l2d::Model::getPixelsPerUnit));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_getSoundFileName(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getSoundFileName");
	LUA_TRY_INVOKE_R(2, static_cast<std::string(l2d::Model::*)(const std::string&, int32_t)>(&l2d::Model::getSoundFileName));
	LUA_INVOKE_FOOTER("2");
}
int lua_l2d_Model_getWind(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:getWind");
	LUA_TRY_INVOKE_R(0, static_cast<cocos2d::Vec2(l2d::Model::*)()const>(&l2d::Model::getWind));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_multiplyParameter(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:multiplyParameter");
	LUA_TRY_INVOKE(3, static_cast<void(l2d::Model::*)(const std::string&, float, float)>(&l2d::Model::multiplyParameter));
	LUA_TRY_INVOKE(2, [](l2d::Model* obj,const std::string& arg0,float arg1){{return obj->multiplyParameter(arg0,std::move(arg1));}});
	LUA_INVOKE_FOOTER("2,3");
}
int lua_l2d_Model_resetDragging(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:resetDragging");
	LUA_TRY_INVOKE(0, static_cast<void(l2d::Model::*)()>(&l2d::Model::resetDragging));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_setAcceleration(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setAcceleration");
	LUA_TRY_INVOKE(3, static_cast<void(l2d::Model::*)(float, float, float)>(&l2d::Model::setAcceleration));
	LUA_INVOKE_FOOTER("3");
}
int lua_l2d_Model_setAutoDragging(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setAutoDragging");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(bool)>(&l2d::Model::setAutoDragging));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setBlinkingInterval(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setBlinkingInterval");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(float)>(&l2d::Model::setBlinkingInterval));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setBlinkingSettings(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setBlinkingSettings");
	LUA_TRY_INVOKE(3, static_cast<void(l2d::Model::*)(float, float, float)>(&l2d::Model::setBlinkingSettings));
	LUA_INVOKE_FOOTER("3");
}
int lua_l2d_Model_setDebugRectEnable(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setDebugRectEnable");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(bool)>(&l2d::Model::setDebugRectEnable));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setDragging(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setDragging");
	LUA_TRY_INVOKE(2, static_cast<void(l2d::Model::*)(float, float)>(&l2d::Model::setDragging));
	LUA_INVOKE_FOOTER("2");
}
int lua_l2d_Model_setExpression(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setExpression");
	LUA_TRY_INVOKE_R(1, static_cast<bool(l2d::Model::*)(const char*)>(&l2d::Model::setExpression));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setGravity(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setGravity");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(const cocos2d::Vec2&)>(&l2d::Model::setGravity));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setLipSync(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setLipSync");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(bool)>(&l2d::Model::setLipSync));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setLipValue(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setLipValue");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(float)>(&l2d::Model::setLipValue));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setModelColor(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setModelColor");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(const cocos2d::Color4B&)>(&l2d::Model::setModelColor));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setModelOpacity(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setModelOpacity");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(uint8_t)>(&l2d::Model::setModelOpacity));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setParameter(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setParameter");
	LUA_TRY_INVOKE(3, static_cast<void(l2d::Model::*)(const std::string&, float, float)>(&l2d::Model::setParameter));
	LUA_TRY_INVOKE(2, [](l2d::Model* obj,const std::string& arg0,float arg1){{return obj->setParameter(arg0,std::move(arg1));}});
	LUA_INVOKE_FOOTER("2,3");
}
int lua_l2d_Model_setPartOpacity(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setPartOpacity");
	LUA_TRY_INVOKE(2, static_cast<void(l2d::Model::*)(const std::string&, float)>(&l2d::Model::setPartOpacity));
	LUA_INVOKE_FOOTER("2");
}
int lua_l2d_Model_setRandomExpression(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setRandomExpression");
	LUA_TRY_INVOKE_R(0, static_cast<bool(l2d::Model::*)()>(&l2d::Model::setRandomExpression));
	LUA_INVOKE_FOOTER("0");
}
int lua_l2d_Model_setSoundEventHandler(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setSoundEventHandler");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(const LAppModel::SoundEventHandler&)>(&l2d::Model::setSoundEventHandler));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_setTouchDragging(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setTouchDragging");
	LUA_TRY_INVOKE(2, static_cast<void(l2d::Model::*)(float, float)>(&l2d::Model::setTouchDragging));
	LUA_INVOKE_FOOTER("2");
}
int lua_l2d_Model_setWind(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:setWind");
	LUA_TRY_INVOKE(1, static_cast<void(l2d::Model::*)(const cocos2d::Vec2&)>(&l2d::Model::setWind));
	LUA_INVOKE_FOOTER("1");
}
int lua_l2d_Model_startMotion(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:startMotion");
	LUA_TRY_INVOKE_R(3, static_cast<bool(l2d::Model::*)(const char*, int32_t, int32_t)>(&l2d::Model::startMotion));
	LUA_INVOKE_FOOTER("3");
}
int lua_l2d_Model_startRandomMotion(lua_State* lua_S)
{
	LUA_INVOKE_HEADER("l2d.Model", "l2d.Model:startRandomMotion");
	LUA_TRY_INVOKE_R(2, static_cast<bool(l2d::Model::*)(const char*, int32_t)>(&l2d::Model::startRandomMotion));
	LUA_INVOKE_FOOTER("2");
}
int lua_l2d_Model_create(lua_State* lua_S)
{
	LUA_SINVOKE_HEADER("l2d.Model", "l2d.Model:create");
	LUA_TRY_INVOKE_R(2, static_cast<l2d::Model*(*)(const std::string&, const std::string&)>(&l2d::Model::create));
	LUA_SINVOKE_FOOTER("2");
}
int luaReg_Live2D_l2dModel(lua_State* lua_S)
{
	LUA_ENTRY("l2d");
	LUA_CLS_DEF_BASE(l2d::Model, "Model", "l2d.Model",
		cocos2d::ui::Widget, "ccui.Widget",
		nullptr, nullptr);
	LUA_METHOD("addParameter", lua_l2d_Model_addParameter);
	LUA_METHOD("areaHitTest", lua_l2d_Model_areaHitTest);
	LUA_METHOD("getCanvasOrigin", lua_l2d_Model_getCanvasOrigin);
	LUA_METHOD("getCanvasSize", lua_l2d_Model_getCanvasSize);
	LUA_METHOD("getDebugRectRenderer", lua_l2d_Model_getDebugRectRenderer);
	LUA_METHOD("getDirectory", lua_l2d_Model_getDirectory);
	LUA_METHOD("getDrawableAlphaBlendMode", lua_l2d_Model_getDrawableAlphaBlendMode);
	LUA_METHOD("getDrawableBlendMode", lua_l2d_Model_getDrawableBlendMode);
	LUA_METHOD("getDrawableCount", lua_l2d_Model_getDrawableCount);
	LUA_METHOD("getDrawableCulling", lua_l2d_Model_getDrawableCulling);
	LUA_METHOD("getDrawableNames", lua_l2d_Model_getDrawableNames);
	LUA_METHOD("getDrawableOpacity", lua_l2d_Model_getDrawableOpacity);
	LUA_METHOD("getExpressionNames", lua_l2d_Model_getExpressionNames);
	LUA_METHOD("getGravity", lua_l2d_Model_getGravity);
	LUA_METHOD("getLipSync", lua_l2d_Model_getLipSync);
	LUA_METHOD("getLipValue", lua_l2d_Model_getLipValue);
	LUA_METHOD("getMocVersion", lua_l2d_Model_getMocVersion);
	LUA_METHOD("getModelColor", lua_l2d_Model_getModelColor);
	LUA_METHOD("getModelOpacity", lua_l2d_Model_getModelOpacity);
	LUA_METHOD("getMotionCount", lua_l2d_Model_getMotionCount);
	LUA_METHOD("getMotionGroupNames", lua_l2d_Model_getMotionGroupNames);
	LUA_METHOD("getParameter", lua_l2d_Model_getParameter);
	LUA_METHOD("getParameterCount", lua_l2d_Model_getParameterCount);
	LUA_METHOD("getParameterDefault", lua_l2d_Model_getParameterDefault);
	LUA_METHOD("getParameterMax", lua_l2d_Model_getParameterMax);
	LUA_METHOD("getParameterMin", lua_l2d_Model_getParameterMin);
	LUA_METHOD("getParameterNames", lua_l2d_Model_getParameterNames);
	LUA_METHOD("getPartCount", lua_l2d_Model_getPartCount);
	LUA_METHOD("getPartNames", lua_l2d_Model_getPartNames);
	LUA_METHOD("getPartOpacity", lua_l2d_Model_getPartOpacity);
	LUA_METHOD("getPartParents", lua_l2d_Model_getPartParents);
	LUA_METHOD("getPixelsPerUnit", lua_l2d_Model_getPixelsPerUnit);
	LUA_METHOD("getSoundFileName", lua_l2d_Model_getSoundFileName);
	LUA_METHOD("getWind", lua_l2d_Model_getWind);
	LUA_METHOD("multiplyParameter", lua_l2d_Model_multiplyParameter);
	LUA_METHOD("resetDragging", lua_l2d_Model_resetDragging);
	LUA_METHOD("setAcceleration", lua_l2d_Model_setAcceleration);
	LUA_METHOD("setAutoDragging", lua_l2d_Model_setAutoDragging);
	LUA_METHOD("setBlinkingInterval", lua_l2d_Model_setBlinkingInterval);
	LUA_METHOD("setBlinkingSettings", lua_l2d_Model_setBlinkingSettings);
	LUA_METHOD("setDebugRectEnable", lua_l2d_Model_setDebugRectEnable);
	LUA_METHOD("setDragging", lua_l2d_Model_setDragging);
	LUA_METHOD("setExpression", lua_l2d_Model_setExpression);
	LUA_METHOD("setGravity", lua_l2d_Model_setGravity);
	LUA_METHOD("setLipSync", lua_l2d_Model_setLipSync);
	LUA_METHOD("setLipValue", lua_l2d_Model_setLipValue);
	LUA_METHOD("setModelColor", lua_l2d_Model_setModelColor);
	LUA_METHOD("setModelOpacity", lua_l2d_Model_setModelOpacity);
	LUA_METHOD("setParameter", lua_l2d_Model_setParameter);
	LUA_METHOD("setPartOpacity", lua_l2d_Model_setPartOpacity);
	LUA_METHOD("setRandomExpression", lua_l2d_Model_setRandomExpression);
	LUA_METHOD("setSoundEventHandler", lua_l2d_Model_setSoundEventHandler);
	LUA_METHOD("setTouchDragging", lua_l2d_Model_setTouchDragging);
	LUA_METHOD("setWind", lua_l2d_Model_setWind);
	LUA_METHOD("startMotion", lua_l2d_Model_startMotion);
	LUA_METHOD("startRandomMotion", lua_l2d_Model_startRandomMotion);
	LUA_METHOD("create", lua_l2d_Model_create);
	LUA_CLS_END();
	LUA_ENTRY_END(1);
	return 0;
}

int register_all_live2d(lua_State* lua_S)
{
	luaReg_Live2D_l2dModel(lua_S);
	return 0;
}
