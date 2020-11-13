def loadStory(filename):
	with open(filename, "r") as f:
		f.readline()
		numOfAI = int(f.readline())
		i = 0
		listOfAI = []
		while(i < numOfAI):
			listOfAI.append(f.readline().rstrip("\n"))
			i += 1
	return ["Player"] + listOfAI


def markComplete(filename):
	with open(filename, "r") as f:
		fileData = f.read()
		fileData = "1"+fileData[1:]
	with open(filename, "w") as f:
		f.seek(0)
		f.write(fileData)
	
	
def isComplete(filename):
	with open(filename, "r") as f:
		complete = f.readline().rstrip("\n")
	if(complete == "1"):
		return True
	else:
		return False

def main(args):
    markComplete("demo.txt")
    x = isComplete("demo.txt")
    print(x)
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
