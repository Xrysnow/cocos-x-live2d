# cocos-x-live2d

This project provides Live2D support for [cocos-x](https://github.com/Xrysnow/cocos-x).

You need to download Cubism SDK for Native from [here](https://www.live2d.com/en/download/cubism-sdk/download-native) and copy files in `Core` folder in the SDK to `Core` folder in this project. For license of usage, please check [here](https://github.com/Live2D/CubismNativeSamples/blob/develop/LICENSE.md).

This project is base on [Cubism Native Samples](https://github.com/Live2D/CubismNativeSamples) and published under [Live2D Open Software License](https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html).

## Cubism SDK version

Use Cubism SDK for Native **5-r.5** (Core `06.00.0001`) together with Framework tag `5-r.5`.
Copy the SDK's Core files directly into `Core/`; keep this repository's `Core/CMakeLists.txt`.
The custom cocos gfx renderer supports Cubism 5.3 color/alpha blend modes and nested offscreens.

`Model:getDrawableBlendMode(name)` returns the Cubism color blend enum: normal, compatible
additive and compatible multiplicative remain `0`, `1` and `2`; new modes use `3` through `17`.
`Model:getDrawableAlphaBlendMode(name)` returns `0` (over), `1` (atop), `2` (out),
`3` (conjoint over) or `4` (disjoint over).

## Example in Lua

```lua
local model = l2d.Model:create('Haru', 'Haru.model3.json')
scene:addChild(model)
model:setPosition(cc.p(500, 500))
model:setTouchEnabled(true)
model:setAutoDragging(true)
model:addClickEventListener(function()
    local pos = model:getTouchEndPosition()
    if model:areaHitTest('Head', pos.x, pos.y) then
        print('hit at head')
    end
end)
```

Remember to call `l2d::Framework::end()` in C++ to end the framework.
