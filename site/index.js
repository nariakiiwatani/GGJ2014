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

	(function() {
		$("#playBoard").empty();
		var w = $(window).width();
		var h = $(window).height();
		var size = (w<h?w:h)/8.+"px";
		for( i=0; i<8; i++){
			$("#playBoard").append("<tr id='tr_"+i+"'></tr>");
			for( j=0; j<8; j++){
				$("#tr_"+i).append("<td id='td_"+i+"_"+j+"'><img src='' alt='' /></td>"); //td_1_2
				$("#td_"+i+"_"+j).css({width: size, height:size});
				$("#td_"+i+"_"+j+" img").attr("width", size).attr("height", size).css("display", "none");
			}
		}
	}());
	
	function showBoard(board, folder){
		for( i=0; i<8; i++){
			for( j=0; j<8; j++){
				switch(board[i][j]){
					case -1:
						$("#td_"+i+"_"+j).css("background-color", "#17607D");
					case 0:
						$("#td_"+i+"_"+j+" img").css("display", "none");
					break;
					default:
						$("#td_"+i+"_"+j+" img").attr("src", folder+board[i][j]+".png").css("display", "inline");
						// objImg.src = "p1/1.png";
						break;
				}
				// var objBody = document.getElementsByTagName("tr").item(i);
				// objBody.appendChild(objTd);
				// objTd.appendChild(objImg);
			}
		}
	}
	function getBoard(){
		$.getJSON("data"+num+".json", function(data) {
			console.info(data["board"]);
			showBoard(data["board"], "p"+num+"/");
		});
	}
	
	setInterval(getBoard, 1*1000);
});