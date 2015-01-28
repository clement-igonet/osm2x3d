//Handle click on any group member
function handleGroupClick(event)
{
    var text;
    var coordinates = event.hitPnt;
    text = "coordX: " + coordinates[0] + "<br>";
    text += "coordY: " + coordinates[1] + "<br>";
    text += "coordZ: " + coordinates[2] + "<br>";
    text += "event: " + event.target.getAttribute('DEF');
    $('#content').html(text);
//    $('#content').html(event.hitPnt[0]);
}

function handleSingleClick(Group) {
    console.log("group", $(Group)[0]);
    var id = $(Group)[0].getAttribute('id')
    var def = $(Group)[0].getAttribute('DEF')
    $('#content').html("<b>Info</b><br>"
            + "id: " + id + "<br>"
            + "DEF: " + def);
}

//$(this).each(function() {
//  $.each(this.attributes, function() {
//    // this.attributes is not a plain object, but an array
//    // of attribute nodes, which contain both the name and value
//    if(this.specified) {
//      console.log(this.name, this.value);
//    }
//  });
//});