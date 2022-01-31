#pragma once

namespace DLP {
    char * normalize( const char* input,
                      size_t len,
                      bool stripAccents,
                      bool lowerCase,
                      bool lumpCharacters,
                      size_t& newLen );
}
