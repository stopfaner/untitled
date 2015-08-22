/*
 * Copyright 2014 untitled project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GAMEINSTANCE
#define GAMEINSTANCE

#include "Sources/gamewidget.h"

/**
 * @brief The GameInstance class - class of main game instance
 * @author -
 */
class GameInstance {

private:

    GameWidget* widget;

    /**
     * @brief GameInstance - contructor of GameInstance class
     */
    GameInstance() {
        widget = new GameWidget();
        //TODO: add startup window logic
    }

    /**
     * @brief GameInstance - copy constructor of GameInstance
     * @param instance - copy of GameInstance instance
     */
    GameInstance(const GameInstance& instance);

    /**
     * @brief operator = - override operator = method
     * @param instance - copy of GameInstance instance
     * @return no return
     */
    GameInstance& operator= (const GameInstance& instance);

public:

    static const GameInstance& getInstance() {
        static GameInstance* instance = new GameInstance();
        return *instance;
    }

    void start() const {
        widget->show();
    }

};


#endif // GAMEINSTANCE

