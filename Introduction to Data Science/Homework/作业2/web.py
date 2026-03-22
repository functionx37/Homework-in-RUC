import re
import os

def extract_titles(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()            
    raw = re.findall(r'<ul class="news-list">(.*?)</ul>', content, re.DOTALL) 
    return re.findall(r'<a href="[^"]*">\s*(.*?)\s*</a>', raw[0], re.DOTALL)
 
titles = []

for file_name in os.listdir('nodes'):   
    file_path = os.path.join('nodes', file_name)
    titles.extend(extract_titles(file_path))

for i, title in enumerate(titles, 1):
    print(f"{i}. {title}")