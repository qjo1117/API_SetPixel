#pragma once

#define SAFEDELETE(ptr) if(ptr != nullptr) { delete ptr; ptr = nullptr; }
// 배열 삭제는 까먹

#define size16(val)		static_cast<int16>(sizeof(val))
#define size32(val)		static_cast<int32>(sizeof(val))
#define len16(arr)		static_cast<int16>(sizeof(arr) / sizeof(arr[0]))
#define len32(arr)		static_cast<int32>(sizeof(arr) / sizeof(arr[0]))

#define OUT

#define CORE			Core::GetInstance()
#define GETRECT			Core::GetInstance()->GetRect()
#define OBJECT			ObjectManager::GetInstance()
#define COLLIDER		ColliderManager::GetInstance()
#define MATH			Math::GetInstance()