document.getElementById("recipient_number").innerHTML = localStorage.getItem("r_number"); 
document.getElementById("transfer_value").innerHTML = localStorage.getItem("t_value");
function post(){
	document.getElementById("recipient").value = localStorage.getItem("r_number"); 
	document.getElementById("value").value = localStorage.getItem("t_value"); 
}
