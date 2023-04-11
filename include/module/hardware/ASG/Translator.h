//
// Created by 11851 on 2023/4/3.
//

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "AsgData.h"
#include "array"

class Translator {
public:
    Translator() = default;

    virtual ~Translator() = default;

    virtual void translate(const AsgData::Sequence *asg_data) = 0;
};

class TranslatorASG8x00 : public Translator {
public:

    typedef std::array<std::vector<long long>, 8> AsgArray;
    typedef std::array<std::vector<long long>, 2> CounterArray;

    TranslatorASG8x00() = default;

    ~TranslatorASG8x00() override = default;

    void translate(const AsgData::Sequence *asg_data) override;

    [[nodiscard]] AsgArray getAsgArray();

    [[nodiscard]] CounterArray getCounterArray();

private:
    void clear();

    void translate(const AsgData::Pulse *asg_data, uint16_t current_loop);

    void translate(const AsgData::Group *asg_data, uint16_t current_loop);

    void translate(const AsgData::Segment *asg_data);

    AsgArray m_asg;
    CounterArray m_counter;

};


#endif //TRANSLATOR_H
