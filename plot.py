from matplotlib import pyplot as plt

X = []
Y = []

with open("data.txt", 'r') as file:
    for line in file.readlines():
        [x, y] = [float(element) for element in line.split()]
        X.append(x)
        Y.append(y/10**6)

fig, xy = plt.subplots(figsize=(8, 4.5), dpi=100)
xy.axis(xmin = 0, xmax = 13, ymin = 0, ymax = 2.5)
xy.set_xlabel('Число потоков parallel_accumulate')
xy.set_ylabel('Время, мс')
xy.plot(X, Y, 'o-', ms = 10, label = "parallel_accumulate")
xy.minorticks_on()
xy.grid(which='major')
xy.grid(which='minor', linestyle = ':')
xy.text(5, 0.35, 'У процессора 6 потоков')
fig.savefig('results.png')