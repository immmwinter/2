#ifndef SCENE_H
#define SCENE_H

#pragma once

enum class SceneState {
    GRAPH,
    PLAYING
};

class SceneManager {
public:
    SceneState currentScene;

    SceneManager() : currentScene(SceneState::GRAPH) {}

    void setScene(SceneState newScene) {
        currentScene = newScene;
    }

    SceneState getScene() const {
        return currentScene;
    }
};

#endif // SCENE_H
