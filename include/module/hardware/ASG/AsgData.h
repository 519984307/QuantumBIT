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

    class Base {
    public:
        explicit Base(std::string name, AsgDataType self_type, int children_type);

        virtual ~Base() = default;

        void appendChildren(std::unique_ptr<Base>&& child);

        void appendChildren(std::vector<std::unique_ptr<Base>> &&children);

        [[nodiscard]] const std::vector<std::unique_ptr<Base>> &children() const;

        void insertChildren(std::unique_ptr<Base>&& child, size_t index);

        void insertChildren(std::vector<std::unique_ptr<Base>> &&children, size_t index);

        [[nodiscard]] virtual json toJson() const = 0;

        void clearChildren();

        [[nodiscard]] std::string getName() const;

        void setName(const char *name);

        void setName(const std::string &name);

        AsgDataType getType()const;

    protected:
        std::vector<std::unique_ptr<Base>> m_children;
        std::string m_name;
        int m_children_type;
        AsgDataType m_type;
    };

    class ChannelData : public Base {
    public:
        ChannelData(NanoSecond duration, NanoSecond increase);

        ~ChannelData() override = default;

        ChannelData(NanoSecond high_time, NanoSecond low_time, NanoSecond high_increase, NanoSecond low_increase);

    public:
        void setDuration(NanoSecond duration);

        void setIncrease(NanoSecond increase);

        void setHighTime(NanoSecond high_time);

        void setLowTime(NanoSecond low_time);

        void setHighIncrease(NanoSecond high_increase);

        void setLowIncrease(NanoSecond low_increase);

        [[nodiscard]] NanoSecond getDuration() const;

        [[nodiscard]] NanoSecond getIncrease() const;

        [[nodiscard]] NanoSecond getHighTime() const;

        [[nodiscard]] NanoSecond getLowTime() const;

        [[nodiscard]] NanoSecond getHighIncrease() const;

        [[nodiscard]] NanoSecond getLowIncrease() const;

        static std::unique_ptr<Base> fromJson(const json &json_data);

        [[nodiscard]] json toJson() const override;

    private:
        NanoSecond m_duration;
        NanoSecond m_increase;
        NanoSecond m_high_time;
        NanoSecond m_low_time;
        NanoSecond m_high_increase;
        NanoSecond m_low_increase;
    };

    class Pulse : public Base {
    public:
        Pulse(NanoSecond duration, NanoSecond increase, uint8_t channel_number = 10);

        ~Pulse() override = default;

        void setDuration(NanoSecond duration);

        void setIncrease(NanoSecond increase);

        [[nodiscard]] NanoSecond getDuration() const;

        [[nodiscard]] NanoSecond getIncrease() const;

        void setHighTime(NanoSecond time, uint8_t index);

        void setLowTime(NanoSecond time, uint8_t index);

        void setHighIncrease(NanoSecond time, uint8_t index);

        void setLowIncrease(NanoSecond time, uint8_t index);

        [[nodiscard]] NanoSecond getHighTime(uint8_t index) const;

        [[nodiscard]] NanoSecond getLowTime(uint8_t index) const;

        [[nodiscard]] NanoSecond getHighIncrease(uint8_t index) const;

        [[nodiscard]] NanoSecond getLowIncrease(uint8_t index) const;

        [[nodiscard]] uint8_t getChannelNumber() const;

        static std::unique_ptr<Base> fromJson(const json &json_data);

        [[nodiscard]] json toJson() const override;

    private:
        Pulse(std::string name, NanoSecond duration, NanoSecond increase, uint8_t channel_number,
              std::vector<std::unique_ptr<Base>> &&children);

        NanoSecond m_duration;
        NanoSecond m_increase;
        NanoSecond m_channel_number;
    };

    class Group : public Base {
    public:
        explicit Group(uint16_t repeat_number);

        ~Group() override = default;

        void setRepeatNumber(uint16_t repeat_number);

        [[nodiscard]] uint16_t getRepeatNumber() const;

        static std::unique_ptr<Base> fromJson(const json &json_data);

        [[nodiscard]] json toJson() const override;

    private:
        Group(std::string name, uint16_t repeat_number, std::vector<std::unique_ptr<Base>> &&children);

        uint16_t m_repeat_number;

    };

    class Segment : public Base {
    public:
        explicit Segment(uint16_t loop);

        ~Segment() override = default;

        void setLoop(uint16_t loop);

        [[nodiscard]] uint16_t getLoop() const;

        static std::unique_ptr<Base> fromJson(const json &json_data);

        [[nodiscard]] json toJson() const override;

    private:
        Segment(std::string name, uint16_t loop, std::vector<std::unique_ptr<Base>>&& children);
        uint16_t m_loop;
    };

    class Sequence : public Base {
    public:
        explicit Sequence(uint8_t channel_number);

        ~Sequence() override = default;

        [[nodiscard]] uint8_t getChannelNumber() const;

        [[nodiscard]] json toJson() const override;

        static std::unique_ptr<Base> fromJson(const json &json_data);

    private:
        uint8_t m_channel_number;

        Sequence(std::string name, uint8_t channel_number, std::vector<std::unique_ptr<Base>> &&children);
    };

}


#endif //ASGDATA_H
