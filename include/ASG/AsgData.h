//
// Created by 11851 on 2023/4/3.
//

#ifndef ASGDATA_H
#define ASGDATA_H

#include "cstdint"
#include "vector"
#include "memory"
#include "string"
#include "nlohmann/json.hpp"

class Translator;
namespace AsgData {
    using nlohmann::json;
    typedef uint32_t NanoSecond;



    enum AsgDataType {
        ChannelDataType = 0x0001,
        PulseType = 0x0002,
        GroupType = 0x0004,
        SegmentType = 0x0008,
        SequenceType = 0x0010,
        NoneType = 0x00
    };

    class ChannelData;
    class Pulse;
    class Group;
    class Segment;
    class Sequence;

    class Base{
    public:
        explicit Base(const char* name, AsgDataType self_type, int children_type);

        virtual ~Base()=default;

        void appendChildren(std::unique_ptr<Base> child);

        void appendChildren(std::vector<std::unique_ptr<Base>>&& children);

        [[nodiscard]] const std::vector<std::unique_ptr<Base>>& children()const;

        void insertChildren(std::unique_ptr<Base> child, size_t index);

        void insertChildren(std::vector<std::unique_ptr<Base>>&& children, size_t index);

        virtual json toJson()const=0;

        virtual void accept(Translator* visitor)=0;

    };
}


#endif //ASGDATA_H
