### 个人代码其他仓库使用的Utils模板

如果你觉得有用，可以点个star⭐️支持一下

## 介绍

本Utils包含以下导入库
nlohmann_json、spdlog、magic_enum、concurrentqueue、imgui
NOTICE: 请看 [这里](https://github.com/masoame/Mortis/blob/master/NOTICE)

## 使用

cmake导入
```cmake
FetchContent_Declare(
    Mortis
    GIT_REPOSITORY  https://github.com/masoame/Mortis.git
    GIT_TAG         xxx # 版本号
)
FetchContent_MakeAvailable(Mortis)
```