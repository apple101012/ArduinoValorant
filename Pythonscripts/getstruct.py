# LOCATE NEEDED VARIABLES
text = open("test1.txt", "r")

pageCount = 0
lineCount = 0

start = None
end = None

usagePages = []
usagePagesStart = []
usagePagesEnd = []
toRead = []

#


# USAGE PAGES
for line in text:
    lineCount += 1

    if "Collection Physical" in line:
        start = lineCount

    if "End Collection" in line:
        end = lineCount

    if start is None:
        pass
    else:
        if lineCount > start:
            if 'Usage Page ' in line:  # if first time set prevPage to current line
                # if 'Undef' not in line:
                pageCount += 1
                usagePages.append(line.split("-> ")[1].split("\n")[0])
                usagePagesStart.append(lineCount)

for x in range(len(usagePagesStart)):
    try:
        toRead.append([usagePagesStart[x], usagePagesStart[x + 1] - 1])
    except IndexError:
        toRead.append([usagePagesStart[x], end])

for x in range(len(toRead)):
    usagePagesEnd.append(toRead[x][1])

print('usagePages: ' + str(usagePages))
print('usagePagesStart: ' + str(usagePagesStart))
print('usagePagesEnd: ' + str(usagePagesEnd))

bLoc = None

for x in range(len(usagePages)):
    if 'Button' in usagePages[x]:
        bLoc = x

# USAGES
usages = []
usagesStart = []
usagesEnd = []
logicalStart = []
first = True

for x in range(len(usagePages)):
    text.seek(0)
    lineCount = 0

    if bLoc == x:
        j = [usagePagesStart[x], usagePagesEnd[x]]

        usages.append('Button')
        usagesStart.append(j[0] + 1)
        usagesEnd.append(j[1] - 1)
        pass
    else:

        j = [usagePagesStart[x], usagePagesEnd[x]]

        for line in text:
            lineCount += 1

            if first:
                if 'Logical Max' in line:
                    logicalStart.append(lineCount)

            if lineCount == end:
                pass

            else:
                if j[0] < lineCount <= j[1]:
                    if 'Usage' in line and 'Usage Page ' not in line:
                        usages.append(line.split("-> ")[1].split('\n')[0])
                        usagesStart.append(lineCount)

                    if 'Input' in line or 'Feature' in line:
                        usagesEnd.append(lineCount)
        first = False

#

# print('usages: ' + str(usages))
# print('usagesStart:   ' + str(usagesStart))


#


# Get usagesEnd right
closest = []
perEnd = 1

first = True
repeat = True
c = 1
bigger = 0
final = False

for x in range(len(usagesStart)):
    # print('x:' + str(x))
    added = False

    if first:
        if usagesStart[x] < usagesEnd[x]:
            closest.append(usagesEnd[x])
            # print('first, ' + str(closest), '\n')

        first = False
        final = False

    else:
        for y in usagesEnd:
            repeat = True
            c = 1

            if not added:
                if y == usagesEnd[0]:
                    pass

                else:
                    try:
                        while repeat:
                            try:
                                if usagesStart[x + c] > y:
                                    bigger = usagesStart[x + c]
                                    repeat = False
                                c += 1

                            except IndexError:
                                final = True
                                repeat = False

                        if final:
                            bigger = 1000

                        if usagesStart[x - 1] < usagesStart[x] < y < bigger:
                            closest.append(y)

                            # print(closest)
                            added = True

                        if added:
                            # print(str(usagesStart[x - 1]) + '<' + str(usagesStart[x]) + '<' + str(y) + '<' + str(
                            # bigger) + '\n')
                            pass

                    except IndexError:
                        if closest[len(closest) - 1] != usagesEnd[len(usagesEnd) - 1]:
                            closest.append(usagesEnd[len(usagesEnd) - 1])
                        added = True
                        # print('except')
                        # print(str(closest) + '\n')

usagesEnd = closest
# print('usagesEnd:     ' + str(usagesEnd) + '\n')


#


# GET REPETITIONS
repetitions = []

oldA = ''
c = 1
count = 1

for a in range(len(usagesEnd)):
    try:
        if usagesEnd[a] == usagesEnd[a + 1]:
            c += 1

        if usagesEnd[a] != usagesEnd[a + 1]:
            repetitions.append(c)
            c = 1

    except IndexError:
        repetitions.append(c)

print(repetitions)

#


# Get usagesStart right

rightStarts = []
c = 0
g = 0

for z in repetitions:
    for x in range(z):
        # print(x)
        try:
            # print('logicalStart: ', logicalStart[c], '<', usagesStart[g])

            if logicalStart[c] < usagesStart[g]:
                rightStarts.append(logicalStart[c])

            else:
                rightStarts.append(usagesStart[g])

            # print(rightStarts)

            g += 1

        except IndexError:

            rightStarts.append(usagesStart[g])
            # print('except ')
            # print(rightStarts)
            pass

    c += 1

print('usages: ' + str(usages))
print('usagesStart:   ' + str(usagesStart))
print('rightStarts:  ', rightStarts)
print('usagesEnd:     ' + str(usagesEnd) + '\n')

usagesStart = rightStarts
#


# ACTUALLY DO STH USEFUL
lineCount = 0
prevSize = 0
prevCount = 0

reportSizeBuf = []
reportCountBuf = []

for x in range(len(usagePages)):
    first = True

    for y in range(len(usages)):
        lineCount = 0
        text.seek(0)

        reportSize = 0
        reportCount = 0

        if usagePagesStart[x] < usagesStart[y] and usagePagesEnd[x] >= usagesEnd[y]:
            # print('went through \n')

            for line in text:
                lineCount += 1

                if usagesStart[y] < lineCount <= usagesEnd[y]:
                    # print(lineCount)

                    if 'Report Size' in line:
                        if first:
                            first = False
                            reportSize += (int("".join(
                                filter(str.isdigit, line.split(' -> ')[1].split('(')[1].split(')')[0]))))

                        else:
                            reportSize += (int("".join(
                                filter(str.isdigit, line.split(' -> ')[1].split('(')[1].split(')')[0]))))

                    if 'Report Count' in line:
                        p = (int("".join(filter(str.isdigit, line.split(' -> ')[1].split('(')[1].split(')')[0]))))

                        if reportCount != p:
                            # print('if')
                            prevCount = p
                            reportCount += (
                                int("".join(filter(str.isdigit, line.split(' -> ')[1].split('(')[1].split(')')[0]))))

            reportSizeBuf.append(reportSize)
            reportCountBuf.append(reportCount)

#

print(reportSizeBuf)
print(reportCountBuf, '\n')

pos = 0
absPos = 0
nameCount = 0
prevSum = 0
c = 0

for x in range(len(repetitions)):
    # print('x: ', x)

    for y in range(repetitions[x]):
        # print('y: ', y)

        # print(c)

        # if prevSum == x+y:
        # prevSum += 1

        txt = usages[nameCount], int(absPos)
        print(txt)

        localSize = reportSizeBuf[c]
        if localSize > 8:
            while (localSize / 8) % 1 != 0:
                localSize += 1

        localCount = reportCountBuf[c]

        # print((localSize / 8 * (localCount / repetitions[x])), ': ', localSize, '/', 8, '*', '(', localCount, '(',
        # c, ')' '/', repetitions[x], ')', '\n')

        if (localSize / 8 * (localCount / repetitions[x])) % 1 != 0:
            while (localSize / 8 * (localCount / repetitions[x])) % 1 != 0:
                localCount += 1

        absPos += localSize / 8 * (localCount / repetitions[x])
        nameCount += 1
        c += 1

text.close()