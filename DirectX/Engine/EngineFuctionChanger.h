﻿#pragma once

#include "IEngineModeChanger.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <vector>

class SpriteButton;

class EngineFuctionChanger {
    using SpriteButtonPtr = std::unique_ptr<SpriteButton>;
    using SpriteButtonPtrArray = std::vector<SpriteButtonPtr>;
    using SpriteFilePathArray = std::vector<std::string>;

public:
    EngineFuctionChanger();
    ~EngineFuctionChanger();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void initialize(IEngineModeChanger* changer);
    void update();
    void draw(const Matrix4& proj) const;

private:
    EngineFuctionChanger(const EngineFuctionChanger&) = delete;
    EngineFuctionChanger& operator=(const EngineFuctionChanger&) = delete;

    void onClickGameButton();
    void onClickMapEditorButton();
    void onClickModelViewerButton();

private:
    IEngineModeChanger* mModeChanger;
    SpriteButtonPtrArray mSpritesButton;
    SpriteFilePathArray mSpritesFilePath;
    Vector2 mStartRenderPosition;
};
