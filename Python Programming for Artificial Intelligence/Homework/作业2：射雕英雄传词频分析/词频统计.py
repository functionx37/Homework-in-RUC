import os
import jieba

with open("小作业3-4-射雕英雄传词典.txt","r",encoding = "UtF-8") as dic_file:
    dictionary = dic_file.readline()
    for it in dictionary:
        jieba.add_word(it)

for file_name in os.listdir("射雕英雄传"):
    file_path = os.path.join("射雕英雄传",file_name)
    with open(file_path,"r",encoding = "UtF-8") as essay:
        content = essay.read()
        count = {}
        max_cnt = 0
        max_word = ""
        word_list = jieba.lcut(content)
        for it in word_list:
            if len(it) == 1:
                continue
            if it in count:
                count[it] += 1
            else:
                count[it] = 1
            if count[it] > max_cnt:
                max_cnt = count[it]
                max_word = it
        print(max_word)