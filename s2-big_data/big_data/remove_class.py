from bs4 import BeautifulSoup

# Read the HTML file
with open("e:/GITHUB/mlaim/s2-big_data/big_data/page.html", "r", encoding="utf-8") as file:
    soup = BeautifulSoup(file, "html.parser")

# Remove all class attributes
for tag in soup.find_all(class_=True):
    del tag["class"]

# Write the updated HTML back
with open("page.html", "w", encoding="utf-8") as file:
    file.write(str(soup))
