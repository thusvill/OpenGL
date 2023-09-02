//
// Created by bios on 9/2/23.
//

#ifndef GAME_CORE_H
#define GAME_CORE_H

#include "memory"


    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    Ref<T> CreateRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }


#endif //GAME_CORE_H
