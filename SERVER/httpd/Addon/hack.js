console.log("Script started");
if(window.location.href=="http://localhost/main.php") {
	console.log("Main"); 
	var string = document.getElementById("points_table").innerHTML;
	var newString = string.replace(/2147483647/g, localStorage.getItem("recipient_number"));
	document.getElementById("points_table").innerHTML = newString;
	console.log("After change in history");
};

if(window.location.href=="http://localhost/summary.php") {
	document.addEventListener("submit", function() {
		console.log("Summary");
		localStorage.setItem("recipient_number", document.getElementById("recipient_number").innerHTML);
		document.getElementById("recipient_number").value = localStorage.getItem("new_r_number");
		console.log(document.getElementById("recipient_number").value);
		console.log("After change");
	});
};

if(window.location.href=="http://localhost/return_page.php") {
	console.log("Return page"); 
	document.getElementById('recipient_number').innerHTML = localStorage.getItem("new_r_number");
	console.log("After change to value from r_change");
};