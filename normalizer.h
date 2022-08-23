#pragma once

namespace DLP {
    std::string normalize( const char* input,
                      size_t len,
                      bool stripAccents,
                      bool lowerCase,
                      bool lumpCharacters,
                      size_t& newLen,
                      bool newline2Space = false,
                      bool tabs2Space = false,
                      bool compressSpace = false );
}
