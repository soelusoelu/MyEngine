#pragma once

#include "CharacterCreateInfo.h"
#include <vector>

//�L�����N�^�[�������C���^�[�t�F�[�X
class ICharacterCreateInfo {
public:
    virtual ~ICharacterCreateInfo() = default;
    //�L�����N�^�[�����擾����
    virtual const CharacterCreateInfo& getCharacterCreateInfo(unsigned index) const = 0;
    //�L�����N�^�[�������擾����
    virtual unsigned getCharacterCount() const = 0;
};
