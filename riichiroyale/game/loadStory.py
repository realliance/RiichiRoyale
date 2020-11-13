def loadStory(filename):
	with open(filename, "r") as f:
		f.readLine()
		numOfAI = int(f.readLine())
		i = 0
		listOfAI = []
		while(i < numOfAI):
			listOfAI.append(f.readLine())
			i += 1
	return ["Player"] + listOfAI


def markComplete(filename):
	with open(filename, "a") as f:
		f.seek(0)
		f.write("1");
	
	
def isComplete(filename):
	with open(filename, "r") as f:
		complete = f.readLine();
	if(complete == "1"):
		return True
	else:
		return False
