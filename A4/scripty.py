import requests
import base64

#curl --user "aifaz.dhuka1:pvrvzfq" http://protest.cpsc.ucalgary.ca/69823/task3.html
url = "https://protest.cpsc.ucalgary.ca/submit.pl"
#color = ["red", "pink", "blue", "green", "violet", "purple", "orange", "yellow", "lime", "black", "white", "indigo","cyan", "magenta"]
#num=[]
'''for value in range(60):
	num.append(str(value))'''
#answer dice= 9
import json
with requests.session() as session:
	#for i in range(1001-90):
	string='{"result":[{"question":"autogen-a4-560","format":"tf","answer":false},{"question":"autogen-a4-561","format":"mc","answer":0},{"question":"autogen-a4-562","format":"line","answer":"cyan"},{"question":"autogen-a4-563","format":"line","answer":"9"},{"question":"autogen-a4-564","format":"line","answer":"15"},{"question":"autogen-a4-565","format":"line","answer":"599"}],"quiz":"task3","practice":false,"total_questions":7,"retry":true,"instant":false,"warnzero":true,"duebefore":"2021-04-11 16:00","idnum":"69823","code":"e9149f0df7e8e87f6824fa5daaedb843","user":"aifaz.dhuka1","declare":"Aifaz Dhuka"}'
	encoding=base64.b64encode(string.encode()).decode()
		#pload = {"result":[{"question":"autogen-a4-560","format":"tf","answer":false},{"question":"autogen-a4-561","format":"mc","answer":1},{"question":"autogen-a4-562","format":"line","answer":"lime"},{"question":"autogen-a4-563","format":"line","answer":"9"},{"question":"autogen-a4-564","format":"line","answer":""},{"question":"autogen-a4-565","format":"line","answer":""},{"question":"autogen-a4-566","format":"mc","answer":-1}],"quiz":"task3","practice":false,"total_questions":7,"retry":true,"instant":false,"warnzero":true,"duebefore":"2021-04-11 16:00","idnum":"69823","code":"e9149f0df7e8e87f6824fa5daaedb843","user":"aifaz.dhuka1","declare":""}
	pload = "data="+encoding
	r=requests.post(url=url,data=pload)
		#print(r.text)
	g=requests.get("https://aifaz.dhuka1:pvrvzfq@protest.cpsc.ucalgary.ca/69823/task3.feedback?t=1617268156839")
		#print(g.text)
	if g.text=="60 / 60":
		print("good")
			#break
		
'''
<div id="question_2" format="line" name="autogen-a4-562">
	<img data-latex=true style="vertical-align:middle" src="/img/7ffe667bb4b2090d4ef92f5d3700bab2c69da56af6161907af5257ff46ef1243.png" 
									alt="What is my favourite colour?"><br>
	<div style="color:red" id="valid_2"></div>
	<input onchange="valid(2)" data-validate="validate_none"  type="text" id="question_2_answer" value=""></div>
<br><hr><h3>Question 4<div id="question_3_grade"></div>
</h3><br>
username= "aifaz.dhuka1"
password="pvrvzfq"
userpass=username+':'+password
encoded=base64.b64encode(userpass.encode()).decode()
headers = {"Authorization" : "Basic %s" % encoded}
'''