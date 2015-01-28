//Handle click on any group member
function handleGroupClick(event)
{
    var text;
    var coordinates = event.hitPnt;
    text = "coordX: " + coordinates[0] + "<br>";
    text += "coordY: " + coordinates[1] + "<br>";
    text += "coordZ: " + coordinates[2] + "<br>";
    
    $('#content').html(text);
//    $('#content').html(event.hitPnt[0]);
}

