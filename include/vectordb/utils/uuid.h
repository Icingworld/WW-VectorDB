#pragma once

#include <array>
#include <functional>
#include <string>

namespace vectordb::utils
{

/**
 * @brief 唯一标识符
 * @version UUIDv4
 */
class UUID
{
public:
    UUID();

    virtual ~UUID() = default;

    /**
    * @brief 创建新的 UUID
    */
    static UUID create();

    /**
    * @brief 从字符串解析 UUID
    * @param str UUID 字符串（支持带/不带分隔符）
    * @throw std::invalid_argument 字符串格式无效
    */
    static UUID fromString(const std::string & str);

    /**
    * @brief 生成唯一标识符
    */
    void generate();

    /**
    * @brief 将唯一标识符转换为字符串
    */
    virtual std::string toString() const;

    bool operator==(const UUID & other) const;

    bool operator!=(const UUID & other) const;

    bool operator<(const UUID & other) const;

    const std::array<unsigned char, 16> & bytes() const;

protected:
    std::array<unsigned char, 16> uuid;        // 128位唯一标识符
};

/**
* @brief 无分隔符的唯一标识符
* @details 用于生成文件名
*/
class NoSeperatorUUID : public UUID
{
public:
    NoSeperatorUUID();

    ~NoSeperatorUUID() override = default;

    /**
    * @brief 将唯一标识符转换为字符串
    */
    std::string toString() const override;
};

} // namespace vectordb::utils

namespace std
{

template <>
struct hash<vectordb::utils::UUID>
{
    std::size_t operator()(const vectordb::utils::UUID & uuid) const noexcept;
};

} // namespace std
