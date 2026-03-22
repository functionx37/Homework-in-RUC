import re

pattern = r'^[a-zA-Z0-9]+(?:\.[a-zA-Z0-9]+)*@[a-zA-Z0-9]+(?:\.[a-zA-Z0-9]+)*\.(?:com|cn)$'
"""
^ 字符串头
[a-zA-Z0-9] 任意字母数字
(?:\.[a-zA-Z0-9]+)* 可选的多个 .xxx, 但每个 . 后必须是字母数字
\. 最后一个 .
(?:com|cn) 必须是 com 或 cn
$ 字符串结尾
"""

test = [
    "34sdg4gfwr4@ruc.edu.cn",
    "653564309@qq.com",
    "wdddangghwz@.com",     # . 前为空
    "10.44.75.123",         # 没有 @，也不是邮箱格式
    "Trer.fsew@sdcs.com",   # 用户名含 .（按规则不允许）
    "hehehe@gmail.com",
    "Sfw$$$dfgdg@126.com",  # 含 $
    "ghhhh@gmail.ussr",     # 不以 com/cn 结尾
    "Chigua@sina.cn",
    "34gdggfdgrtyhyr@163.com"
]

for email in test:
    if re.match(pattern, email):
        print(f"{email} 合法")
    else:
        print(f"{email} 不合法")