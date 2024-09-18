//
// Created by merika on 9/18/2024.
//

#ifndef TFILESAVERESULT_HPP
#define TFILESAVERESULT_HPP

#include <string>

namespace forza_saveswapper {

    struct TFileSaveResult {
        bool Succeeded = false;
        std::string Path;
    };

} // forza_saveswapper

#endif //TFILESAVERESULT_HPP
