document.addEventListener('DOMContentLoaded', function () {

  // Check if the user is logged in when the page loads
  var isLoggedIn = document.cookie.includes("loggedIn=true");
  if (!isLoggedIn) {
    alert("Sorry, please login");
    window.location.href = "../authentication.html";
  }

  // Function to handle user sign out
  const userSignOut = () => {
    document.cookie = "loggedIn=false";
    alert("You have been signed out");
  };

  // Add an event listener to the signOutButton
  const signOutButton = document.querySelector("#signOutButton");
  signOutButton.addEventListener('click', userSignOut);
});
