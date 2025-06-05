import os
import jieba

with open("小作业3-4-射雕英雄传词典.txt","r",encoding = "UtF-8") as dic_file:
    dictionary = dic_file.readline()
    for it in dictionary:
        jieba.add_word(it)

name_list = []

with open("小作业3-4-射雕英雄传人物.txt","r",encoding = "UTF-8") as name:
    for line in name:
        name_list.append(line.rstrip('\n'))

file_list = []

cnt_list = {}

for file_name in os.listdir("射雕英雄传"):
    file_list.append(file_name[0:-4])
file_list.remove("后记")
file_list.append("后记")

for file_name in file_list:
    file_path = os.path.join("射雕英雄传", file_name+".txt")
    cnt = {}
    with open(file_path,"r",encoding = "UtF-8") as essay:
        content = essay.read()
        word_list = jieba.lcut(content)
        for it in word_list:
            if it in name_list:
                try:
                    cnt[it] += 1
                except:
                    cnt[it] = 1                    
    cnt_list[file_name] = cnt

with open("人名表.csv","w",encoding = "UTF-8") as excel:
    for file in file_list:
        excel.write(",")
        excel.write(file)
    excel.write("\n")
    for name in name_list:
        excel.write(name)
        for file in file_list:
            excel.write(",")
            try:
                excel.write(str(cnt_list[file][name]))
            except:
                excel.write("0")
        excel.write('\n')