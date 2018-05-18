$(function(){
    $("#btn").mousedown(function(){
        $("#pass").attr("type", "text");
    });
    $("#btn").mouseup(function(){
        $("#pass").attr("type", "password");
    });
});
