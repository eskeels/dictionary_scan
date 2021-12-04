namespace DLP {

    class DictionaryTermId {
        public:
            DictionaryTermId(uint16_t dictId, uint16_t termId) {
                Id_ = MakeDictTermId(dictId, termId);
            }

            DictionaryTermId(uint32_t id) {
                Id_ = id;
            }

            uint32_t MakeDictTermId(uint16_t dictId, uint16_t termId) {
                uint32_t id = dictId;
                id <<= 16;
                id |= termId;
                return id;
            }

            uint16_t GetDictionaryId() const {
                return Id_ >> 16;
            }

            uint16_t GetTermId() const {
                return (uint16_t) Id_;
            }

            uint32_t GetId() const {
                return Id_;
            }

        protected:
            uint32_t Id_ = 0;
    };
}

