const chatbotToggler = document.querySelector(".chatbot-toggler");
const closeBtn = document.querySelector(".close-btn");
const chatbox = document.querySelector(".chatbox");
const chatInput = document.querySelector(".chat-input textarea");
const sendChatBtn = document.querySelector(".chat-input span");

let userMessage = null; // Variable to store user's message
let responses = []; // Array to store multiple responses
const inputInitHeight = chatInput.scrollHeight;

const createChatLi = (message, className) => {
    // Create a chat <li> element with passed message and className
    const chatLi = document.createElement("li");
    chatLi.classList.add("chat", `${className}`);
    let chatContent = className === "outgoing" ? `<p></p>` : `<span class="material-symbols-outlined">smart_toy</span> <p></p>`;
    chatLi.innerHTML = chatContent;
    chatLi.querySelector("p").textContent = message;
    return chatLi; // return chat <li> element
}

const handleChat = () => {
  userMessage = chatInput.value.trim().toLowerCase(); // Get user entered message and remove extra whitespace
  if (!userMessage) return;

  // Clear the input textarea and set its height to default
  chatInput.value = "";
  chatInput.style.height = `${inputInitHeight}px`;

  // Append the user's message to the chatbox
  chatbox.appendChild(createChatLi(userMessage, "outgoing"));
  chatbox.scrollTo(0, chatbox.scrollHeight);

  setTimeout(() => {
      let response = ""; // Default response

      // Check if the user's message contains a specific command
      if (
          userMessage.includes("1") ||
          userMessage.includes("website purpose") ||
          userMessage.includes("Purpose")
      ) {
          response = "Our goal is to integrate healthcare related sensors to our website, providing real-time insights and useful health tips.";
      } else if (
          userMessage.includes("2") ||
          userMessage.includes("website guide") ||
          userMessage.includes("guide")
      ) {
          response = "The Dashboard offers a live display of your health data.  \n\nExercise Page has different classes of exercise for you. Discover Page features official government health website and health tips. \n\nSettings page allow you to save ur data.";
      } else if (
          userMessage.includes("3") ||
          userMessage.includes("website infrastructure") ||
          userMessage.includes("infrastructure")
      ) {
          response = "Our website is hosted on Firebase Web Hosting, and used Firebase Authentication to manage our security. \n\nOur Dashboard Data is stored in Firebase Realtime Database. \n\nOur user details are stored in Firebase Firestore.";
      } else if (
        userMessage.includes("help")
    ) {
        response = "Here is a list of questions you can ask!\n1. Website Purpose\n2. Website Guide\n3. Website Infrastructure";
      } else if (
        userMessage.includes("hi") ||
        userMessage.includes("hello")
    ) {
        response = "Hi there 👋 !help for guide" ;
    }else if (!responses.length) {
          // Add the help response only if there are no responses yet
          response = "Im sorry. I dont quite understand";
      }

      // Display the response
      const incomingChatLi = createChatLi(response, "incoming");
      chatbox.appendChild(incomingChatLi);
      chatbox.scrollTo(0, chatbox.scrollHeight);
  }, 600);
}


const displayResponses = () => {
    responses.forEach((response, index) => {
        setTimeout(() => {
            const incomingChatLi = createChatLi(response, "incoming");
            chatbox.appendChild(incomingChatLi);
            chatbox.scrollTo(0, chatbox.scrollHeight);
        }, index * 600); // Adjust the delay as needed
    });
}

chatInput.addEventListener("input", () => {
    // Adjust the height of the input textarea based on its content
    chatInput.style.height = `${inputInitHeight}px`;
    chatInput.style.height = `${chatInput.scrollHeight}px`;
});

chatInput.addEventListener("keydown", (e) => {
    // If Enter key is pressed without Shift key and the window 
    // width is greater than 800px, handle the chat
    if (e.key === "Enter" && !e.shiftKey && window.innerWidth > 800) {
        e.preventDefault();
        handleChat();
    }
});

sendChatBtn.addEventListener("click", handleChat);
closeBtn.addEventListener("click", () => document.body.classList.remove("show-chatbot"));
chatbotToggler.addEventListener("click", () => document.body.classList.toggle("show-chatbot"));
