#include<PE.h>
#include<iostream>

#include <iostream>
#include <string>
#include <map>
#include <utility>

constexpr auto& BASE_WORDS = L"一十人入八儿九刀力又小了厂卜几丁七乃万才寸下上口土士大女子也己已巳干工弋三于亏丈夫天无元云专扎艺木五支厅不太犬区历尤友匹车巨牙屯比互切瓦止少日中冈贝内水见午牛手毛气升长仁什片仆化仇币仍仅斤爪反介父从今凶分乏公仓月氏勿欠风丹匀乌凤勾文六方火为斗忆订计户认心尺引丑巴孔队办以允予劝双你我他这里外前后东来去有是非对错好坏高矮胖瘦短远近快慢早晚明昨年时地图和同跟与或及在再就还更最很极常都全总只单独特每各别另其怎因果间旁边关张合收存放给拿送接交换买卖钱财物品用具衣服帽鞋袜裤衫裙被枕床椅桌凳柜箱门窗墙房屋家园校场道街巷桥";
constexpr std::wstring_view DECORATION_WORDS = L"哈基米|那没撸多|阿西噶压|库路曼波|哦吗吉利|南北绿豆|椰奶龙";

class HaJimiEncodeUtil {

    inline static const std::vector<wchar_t>& dictionary() {
        static std::vector<wchar_t> _dictionary;
        if (_dictionary.empty()) {
            for (const auto& ch : BASE_WORDS) {
                _dictionary.push_back(ch);
            }
		}
    }

    inline static const std::vector<std::wstring>& decoration() {
        static std::vector<std::wstring> _decoration;
        if(_decoration.empty()) {
            size_t start = 0;
            size_t end = DECORATION_WORDS.find(L'|');
            while (end != std::wstring::npos) {
                _decoration.emplace_back(DECORATION_WORDS.substr(start, end - start).data(), end - start);
                start = end + 1;
                end = DECORATION_WORDS.find(L'|', start);
            }
            _decoration.emplace_back(DECORATION_WORDS.substr(start).data());
		}

    }

    static std::map<std::wstring, std::size_t>& charToIndexMap() {
        static std::map<std::wstring, std::size_t> _charToIndexMap;
        if (_charToIndexMap.empty()) {
            for (std::size_t i = 0; i != decoration().size(); ++i) {
                _charToIndexMap[decoration()[i]] = i;
            }
        }
    }

    static auto encode(std::vector<UINT8> str) -> std::wstring {
        auto viewString = str | std::views::transform([](const UINT8& uInt) {
            return dictionary()[uInt];
        });
        return { viewString.begin(),viewString.end() };
    }


    static auto decode(const std::wstring& chr) -> std::vector<UINT8> {

		std::vector<UINT8> result;
        for (const auto& c : chr | std::views::transform([](const wchar_t& c) { return std::wstring{ c }; })) {
            auto& charMap = charToIndexMap();
            if (charMap.contains(c)) {
                result.push_back(charMap[c]);
            }
            
        }
             
    }


        /**
         * “哈基密语”字符串解码成 Uint8Array
         */
    static decode(text: string) : Uint8Array{
        const arr = []
        for (const ch of text) {
        const index = this.charToIndexMap.get(ch)
        if (index == = undefined) {
            throw new Error(`未知字符: ${ch}`)
        }
        arr.push(index)
        }
        return new Uint8Array(arr)
    }

                  /**
                   * 把“哈基密语”装饰一下
                   */
                  static decorateHaJimi(text: string) : string{
                    const chars = text.split('')
                    const insertions : { pos: number, fragment : string }[] = []
                  // 插入开头
                  insertions.push({
                    pos: 0,
                    fragment : this.decoration[Math.floor(Math.random() * this.decoration.length)],
                  })
                  // 中间插入，按间隔 5~15 插入
                  let i = 0
                  while (i + 5 < chars.length) {
                    const step = Math.floor(Math.random() * 11) + 5 // [5, 15]
                    i += step
                    if (i >= chars.length - 5)
                      break // 剩下不足5个就不插
                    insertions.push({
                      pos: i,
                      fragment : this.decoration[Math.floor(Math.random() * this.decoration.length)],
                    })
                  }
    // 插入结尾
    insertions.push({
      pos: chars.length,
      fragment : this.decoration[Math.floor(Math.random() * this.decoration.length)],
    })
    // 从后往前插，避免位置错乱
    insertions.sort((a, b) = > b.pos - a.pos)
    for (const { pos, fragment } of insertions) {
      chars.splice(pos, 0, fragment)
    }
    return chars.join('')
                  }

                  /**
                   * 装饰：哈基密钥
                   */
                  static decorateHaJimiKey(text: string) : string{
                    return `哈基密钥${this.decorateHaJimi(text)}`
                  }

                  /**
                   * 装饰：哈基密语
                   */
                  static decorateHaJimiText(text: string) : string{
                    return `哈基密语${this.decorateHaJimi(text)}`
                  }

                  /**
                   * 装饰：哈基密密语
                   */
                  static decorateHaJimiTextCompress(text: string) : string{
                    return `哈基密密语${this.decorateHaJimi(text)}`
                  }

                  /**
                   * 把装饰过的“哈基密语”给还原
                   */
                  static stripHaJimi(text: string) : string{
                    const set = new Set(`哈基密钥语${this.decoration.join('')}`)
                    let result = ''
                    for (const ch of text) {
                      if (!set.has(ch)) {
                        result += ch
                      }
                    }
                    return result
                        }
}

//static decorateHaJimi(text: string) : string{
//        const chars = text.split('')
//        const insertions : { pos: number, fragment : string }[] = []
//        // 插入开头
//        insertions.push({
//            pos: 0,
//            fragment : this.decoration[Math.floor(Math.random() * this.decoration.length)],
//        })
//        // 中间插入，按间隔 5~15 插入
//        let i = 0
//        while (i + 5 < chars.length) {
//            const step = Math.floor(Math.random() * 11) + 5 // [5, 15]
//            i += step
//            if (i >= chars.length - 5)
//            break // 剩下不足5个就不插
//            insertions.push({
//            pos: i,
//            fragment : this.decoration[Math.floor(Math.random() * this.decoration.length)],
//            })
//        }
//        // 插入结尾
//        insertions.push({
//        pos: chars.length,
//        fragment : this.decoration[Math.floor(Math.random() * this.decoration.length)],
//        })
//        // 从后往前插，避免位置错乱
//        insertions.sort((a, b) = > b.pos - a.pos)
//        for (const { pos, fragment } of insertions) {
//        chars.splice(pos, 0, fragment)
//        }
//        return chars.join('')
//}

std::wstring decorateHaJimi(std::wstring_view str)
{
	std::vector<std::tuple<long long, std::string>> insertions;
	//insertions.push_back(std::make_tuple(0, DECORATION_WORDS + std::to_string(rand() % 7)));
}

int main()
{
	std::cout << sizeof(BASE_WORDS) << std::endl;
}