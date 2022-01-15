# 读取lng2.csv中的数据，经过预处理分类输出到各个文件中
f = open('lng2.csv', 'r+')
f1 = open('in.csv', 'w+')
f2 = open('out.csv', 'w+')
f3 = open('mooring.csv', 'w+')

is_first = True  # 是否为第一个有效数据
draft, last_draft = 0, 0  # 当前吃水深度和上一个吃水深度
for line in f.readlines():
    line = line.split()
    draft = int(line[6])
    if draft == 0 or draft > 300:
        continue

    if is_first == True:
        is_first = False
        f3.write(' '.join(line) + '\n')
    # 如果小于上一个吃水深度，则加入入站
    elif draft < last_draft:
        f1.write(' '.join(line) + '\n')
    # 如果大于上一个吃水深度，则加入出战
    elif draft > last_draft:
        f2.write(' '.join(line) + '\n')
    # 如果等于上一个吃水深度，则加入锚地
    elif draft == last_draft:
        f3.write(' '.join(line) + '\n')
    # 更新上一个吃水深度
    last_draft = draft

f.close()
f1.close()
f2.close()
f3.close()
