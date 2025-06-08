#pragma once
#include <glm/glm.hpp>
#include <algorithm>

namespace GameConfig
{
    struct Config
    {
        glm::vec2 BrickSize;
        float BrickPadding;
        float BrickMarginLeft, BrickMarginRight;
        float BrickMarginTop, BrickMarginBottom;
        int BrickRows;
        int BricksPerRow;
        glm::vec3 BrickColor;

        glm::vec2 PaddleSize;
        glm::vec2 PaddleStartPos;
        float PaddleSpeed;
        glm::vec3 PaddleColor;

        glm::vec2 BallSize;
        glm::vec2 BallStartPos;
        glm::vec2 BallInitialVelocity;
        float BallMaxSpeed;
        glm::vec3 BallColor;
    };

    inline Config ForLevel(int level)
    {
        Config cfg;

        int baseRows = 3;
        int maxRows = 10;
        cfg.BrickRows = std::min(baseRows + level, maxRows);

        int baseCols = 10;
        cfg.BricksPerRow = baseCols; // could vary later

        cfg.BrickSize = glm::vec2(60.0f, 20.0f);
        cfg.BrickPadding = 12.0f;

        cfg.BrickMarginLeft = 40.0f;
        cfg.BrickMarginRight = 40.0f;
        cfg.BrickMarginTop = 40.0f;
        cfg.BrickMarginBottom = 200.0f;

        cfg.BrickColor = glm::vec3(
            0.5f + 0.5f * std::sin(level * 0.5f),
            0.5f + 0.5f * std::sin(level * 0.3f + 1.0f),
            0.5f + 0.5f * std::sin(level * 0.7f + 2.0f)
        );

        cfg.PaddleSize = glm::vec2(100.0f - level * 5.0f, 20.0f); // gets smaller
        cfg.PaddleStartPos = glm::vec2(400.0f, 50.0f);
        cfg.PaddleSpeed = 200.0f + level * 10.0f;
        cfg.PaddleColor = glm::vec3(0.2f, 0.6f, 1.0f);

        cfg.BallSize = glm::vec2(16.0f);
        cfg.BallStartPos = glm::vec2(400.0f, 300.0f);
        cfg.BallInitialVelocity = glm::vec2(40.0f + level * 5.0f, -40.0f - level * 5.0f);
        cfg.BallMaxSpeed = 150.0f;
        cfg.BallColor = glm::vec3(1.0f);

        return cfg;
    }
}
