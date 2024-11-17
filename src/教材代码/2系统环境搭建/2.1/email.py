import re

def find_email(text):
    email_pattern = r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b'
    matches = re.findall(email_pattern,text)
    return matches

text = "我的电子邮件是example@gmail.com,你可以了联系我。"

emails = find_email(text)
for email in emails:
    print(email)