$(function() {
	var query = function() {
		var ret = {};
		var q = (location.search!='')?location.search.substring(1).split('&'):[];
		for(var i = 0; i < q.length; ++i) {
			var set = q[i].split('=');
			if(set.length >= 2) {
				ret[set[0]] = set[1];
			}
		}
		return ret;
	}();
	var num = query["p"]?query["p"]:0;
	$.getJSON("data"+num+".json", function(data) {
		console.info(data["board"]);
	});


});