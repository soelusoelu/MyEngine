#pragma once

//エネミーエイリアス宣言用ヘッダ

#include <memory>
#include <vector>

class OctopusFoot;

using OctopusFootPtr = std::shared_ptr<OctopusFoot>;
using OctopusFootPtrArray = std::vector<OctopusFootPtr>;

using AliveNumbers = std::vector<unsigned>;
