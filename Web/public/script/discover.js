
document.getElementById("card1").onclick = function() {
  window.open("https://www.healthhub.sg/", "_blank");
 };

 document.getElementById("card2").onclick = function() {
  window.open("https://www.healthiersg.gov.sg/", "_blank");
 };

 document.getElementById("card3").onclick = function() {
  window.open("https://www.moh.gov.sg/", "_blank");
 };

 document.getElementById("card4").onclick = function() {
  window.open("https://www.singhealth.com.sg/", "_blank");
 };

 document.getElementById("card5").onclick = function() {
  window.open("https://hpb.gov.sg/", "_blank");
 };

 document.getElementById("card6").onclick = function() {
  window.open("https://www.hsa.gov.sg/", "_blank");
 }; 

 document.getElementById("card7").onclick = function() {
  window.open("https://www.healthcarecorps.gov.sg/", "_blank");
 }; 

const apiKey = "10da7dff9deb4dfcaa1b435af9e55f0a";
const apiUrl = `https://newsapi.org/v2/top-headlines?country=sg&category=health&apiKey=${apiKey}`;

fetch(apiUrl)
  .then(response => response.json())
  .then(data => {
    if (data.status === "ok") {
      const newsElements = [
        {
          title: document.getElementById("title1"),
          description: document.getElementById("description1"),
          url: document.getElementById("url1"),
          source: document.getElementById("source1"),
          date: document.getElementById("date1")
        },
        {
          title: document.getElementById("title2"),
          description: document.getElementById("description2"),
          url: document.getElementById("url2"),
          source: document.getElementById("source2"),
          date: document.getElementById("date2")
        },
        {
          title: document.getElementById("title3"),
          description: document.getElementById("description3"),
          url: document.getElementById("url3"),
          source: document.getElementById("source3"),
          date: document.getElementById("date3")
        }
      ];

      for (let i = 0; i < Math.min(3, data.articles.length); i++) {
        const article = data.articles[i];
        const newsElement = newsElements[i];

        newsElement.title.textContent = article.title;

        // Check if the description is null or empty
        if (!article.description) {
          // Set a placeholder image when description is null
          newsElement.description.innerHTML = '<img src="" alt="No Description">';
        } else {
          // Set the actual description when it's not null
          newsElement.description.textContent = article.description;
        }

        // Create an anchor element for the URL
        const urlAnchor = document.createElement("a");
        urlAnchor.textContent = "Click here";
        urlAnchor.href = article.url;
        urlAnchor.target = "_blank";
        
        // Apply styling to make the anchor appear in blue
        urlAnchor.style.color = "blue";
        urlAnchor.style.textDecoration = "underline";

        // Clear the existing content and append the anchor element
        newsElement.url.innerHTML = "";
        newsElement.url.appendChild(urlAnchor);

        newsElement.source.textContent = `Source: ${article.source.name}`;
        newsElement.date.textContent = `Published At: ${article.publishedAt}`;
      }
    } else {
      console.error("Error fetching news data:", data.message || "Unknown error");
    }
  })
  .catch(error => console.error("Error fetching news data:", error));
