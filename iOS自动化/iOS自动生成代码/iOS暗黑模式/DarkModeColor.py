#!/usr/local/bin/python3
# coding=utf-8

import os, json, re, sys


# eg: UIColor.hex(0x1f1c19)
# group1: UIColor.hex(0x
# group2: 1f1c19
UICOLOR_HEX = re.compile(r'(UIColor\.hex\(0x)([0-9A-Fa-f]{6,8})\)')
# eg: UIColor.hex(0x1f1c19, alpha: 0.3)
# group1: UIColor.hex(0x
# group2: 1f1c19
# group3: , alpha: 
# group4: 0.3
UICOLOR_HEX_ALPHA = re.compile(r'(UIColor\.hex\(0x)([0-9A-Fa-f]{6,8})(,\s*alpha:\s*)(1|0.[0-9]+)\)')

# iOS Color Asset 中的颜色抽象
class ColorAssetEntity(object):

    # 通过 csv 的一行文本创建, eg. "black_bar,#000000,#D8D8D8"
    def __init__(self, lineStr):
        components = lineStr.rstrip('\n').rstrip('\r').replace(' ', '').split(',')
        if len(components) < 3:
            return None
        # gray01 这样
        self.name = components[0]
        # #EF38AD / #EF392443
        self.default_value = components[1].strip('#')
        # #EF38AD / #EF392443
        self.dark_mode_value = components[2].strip('#')

    # 从十六进制字符串解析出 颜色 RGBA，返回结果为 tuple (r, g, b, a)
    # 支持 #AABBCC/#AABBCCDD/AABBCC/AABBCCDD 格式
    @staticmethod
    def extract_rgba_from_hex_string(hex_str):
        hex_str = hex_str.strip('#')
        if len(hex_str) == 6:
            return (hex_str[0:2], hex_str[2:4], hex_str[4:6], "FF")
        elif len(hex_str) == 8:
            return (hex_str[0:2], hex_str[2:4], hex_str[4:6], hex_str[6:8])
        return ("FF", "FF", "FF", "FF")

    # 转化成 .colorset 需要的字典格式
    def to_value_dict(self):
        value_dict = {}
        (r, g, b, a) = ColorAssetEntity.extract_rgba_from_hex_string(self.default_value)
        (dark_r, dark_g, dark_b, dark_a) = ColorAssetEntity.extract_rgba_from_hex_string(self.dark_mode_value)
        value_dict['info'] = {'version':1, 'author':'xcode'}
        value_dict['colors'] = [
            {
                "idiom" : "universal",
                "color" : {
                    "color-space" : "srgb",
                    "components" : {
                        "red" : "0x%s" % r,
                        "alpha" : "0x%s" % a,
                        "blue" : "0x%s" % b,
                        "green" : "0x%s" % g
                    }
                }
            },
            {
                "idiom" : "universal",
                "appearances" : [
                    {
                        "appearance" : "luminosity",
                        "value" : "dark"
                    }
                ],
                "color" : {
                    "color-space" : "srgb",
                    "components" : {
                        "red" : "0x%s" % dark_r,
                        "alpha" : "0x%s" % dark_a,
                        "blue" : "0x%s" % dark_b,
                        "green" : "0x%s" % dark_g
                    }
                }
            }
        ]
        return value_dict

    # 将颜色保存到指定路径下的 {name}.colorset 中
    def export_colorset_to_path(self, path):
        color_folder = os.path.join(path, (self.name + '.colorset'))
        if not os.path.exists(color_folder):
            os.mkdir(color_folder)
        content = json.dumps(self.to_value_dict())
        open(os.path.join(color_folder, "Contents.json"), 'wt', encoding='utf-8').write(content)
        pass    

    pass

# 从 .csv 中读取设计提供的颜色配置列表, 返回 [ColorAssetEntity]
def load_color_list_from_csv_file(filepath):
    color_list = []
    f = open(filepath)
    for line in f:
        if "#" not in line:
            continue
        entity = ColorAssetEntity(line)
        if entity != None:
            color_list.append(entity)
    f.close()
    return color_list

# 将颜色配置列表写入到指定的 xcasset_path 中
def export_color_list_to_xcasset_path(color_list, filepath):
    if not os.path.exists(filepath):
        os.mkdir(filepath)
    for entity in color_list:
        entity.export_colorset_to_path(filepath)
    pass

# 通过 color_list 创建 color_map
# key: 默认颜色的 RGBA 值, eg. "EEFF33"
# value: 颜色名称，eg. ”gray_01“
# 规则: 由于不同名称的默认颜色可能是同一个 RGBA 值，这里简化处理，使用名称最短的那个
def create_rgba2name_by_color_list(color_list):
    rgba2name = {}
    for entity in color_list:
        rgba = entity.default_value.upper()
        if rgba not in rgba2name:
            rgba2name[rgba] = entity.name
        elif len(rgba2name[rgba]) > len(entity.name):
            rgba2name[rgba] = entity.name
        pass
    return rgba2name

def transfer_alpha_to_hex_str(alpha):
    return "%02X" % int(float(alpha) * 255)

# 查找 swift 文件中颜色的使用代码，检查颜色是否在 color_map 中，在的话替换为 R.color.colorName 的方式
def find_and_replace_color_code_in_swift_file(filepath, rgba2name):
    changed = False
    def change_matched_hex_color(match):
        nonlocal changed
        rgba = match[2].upper()
        if match.lastindex >= 4 :
            # 带着 alpha 的
            alpha_str = transfer_alpha_to_hex_str(match[4])
            if alpha_str != 'FF':
                rgba += alpha_str
                pass
        if rgba in rgba2name:
            # print(color_rgba)
            changed = True
            return "R.color." + rgba2name[rgba] + '()!'
        else:
            print("no matched color: #%s" % rgba)
        return match[0]

    changed_codes = re.sub(UICOLOR_HEX_ALPHA, change_matched_hex_color, open(filepath, 'rt', encoding='utf-8').read())
    changed_codes = re.sub(UICOLOR_HEX, change_matched_hex_color, changed_codes)

    if changed:
        # 将源码的改动写回到文件
        open(filepath, 'wt', encoding='utf-8').write(changed_codes)
        pass
    pass

# 在指定的 iOS 目录下，遍历所有的 .swift 文件，
# 查找 color_list 中颜色的裸式使用，替换为 R.color.colorName 的方式
def replace_code_by_color_list_in_ios_project(color_list, ios_project_path):
    rgba2name = create_rgba2name_by_color_list(color_list)
    for root, dirs, files in os.walk(os.path.join(ios_project_path)):
        for file in files:
            if os.path.splitext(file)[1].lower() in ['.swift']:
                find_and_replace_color_code_in_swift_file(os.path.join(root, file), rgba2name)
    pass

# 传入三个参数
# csv_file_path: 颜色配置表的 csv 文件
# xcasset_path: iOS 工程用的颜色配置文件, eg. xx/xx/color.xcasset
# ios_project_path: iOS 工程主目录，用来替换 swift 中的颜色代码
if __name__ == "__main__":
    if len(sys.argv) < 4:
        print("Usage: \r\n\t%s csv_file_path xcasset_path ios_project_path" % (sys.argv[0]))
        exit(1)
    
    csv_file_path = sys.argv[1]
    xcasset_path = sys.argv[2]
    ios_project_path = sys.argv[3]

    color_list = load_color_list_from_csv_file(csv_file_path)

    if len(color_list) == 0:
        print("some thing wrong! no color data in " + csv_file_path)
        exit(1)

    # for color in color_list:
    #     print("%s, %s, %s" % (color.name, color.default_value, color.dark_mode_value))
    #     pass
    export_color_list_to_xcasset_path(color_list, xcasset_path)

    replace_code_by_color_list_in_ios_project(color_list, ios_project_path)
    pass
