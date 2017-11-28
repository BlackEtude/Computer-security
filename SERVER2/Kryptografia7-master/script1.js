function storeData(){
	if (typeof(Storage) !== "undefined") {
	localStorage.setItem("r_number", document.forms[0].elements["recipient"].value);
	localStorage.setItem("t_value", document.forms[0].elements["value"].value);
	} else {
    console.log("No storage support!");
	}
}
	