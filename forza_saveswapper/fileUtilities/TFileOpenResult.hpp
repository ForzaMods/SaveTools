//
// Created by merika on 9/17/2024.
//

#ifndef TFILEOPENRESULT_HPP
#define TFILEOPENRESULT_HPP

#include <string>

namespace forza_saveswapper {

    struct TFileOpenResult {
        bool Succeeded = false;
        std::string Path;
        std::string SelectedFileName;
    };

}

#endif //TFILEOPENRESULT_HPP
