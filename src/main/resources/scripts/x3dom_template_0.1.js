
var groupMesh = 'group_ME_';



Array.max = function( array ){
    return Math.max.apply( Math, array );
};
 
Array.min = function( array ){
    return Math.min.apply( Math, array );
};


function axisVec(axis) {
    if (axis === undefined)
        axis = "posY";

    switch (axis) {
        case "posX":
            n0 = new x3dom.fields.SFVec3f(1, 0, 0);
            break;
        case "negX":
            n0 = new x3dom.fields.SFVec3f(-1, 0, 0);
            break;
        case "posY":
            n0 = new x3dom.fields.SFVec3f(0, 1, 0);
            break;
        case "negY":
            n0 = new x3dom.fields.SFVec3f(0, -1, 0);
            break;
        case "posZ":
            n0 = new x3dom.fields.SFVec3f(0, 0, 1);
            break;
        case "negZ":
            n0 = new x3dom.fields.SFVec3f(0, 0, -1);
            break;
    }
    return n0;  
}


// modified from x3dom.Runtime.showObject() by AH
x3dom.Runtime.prototype.showLayer = function(mat,min,max,axis)
{
    if (mat)
    {
        var viewarea = this.canvas.doc._viewarea;

        // assume FOV_smaller as camera's fovMode
        var focalLen = (viewarea._width < viewarea._height) ?
                        viewarea._width : viewarea._height;

        var n0 = axisVec(axis);
        var viewpoint = this.canvas.doc._scene.getViewpoint();
        var fov = viewpoint.getFieldOfView() / 2.0;
        var ta = Math.tan(fov);

        if (Math.abs(ta) > x3dom.fields.Eps) {
            focalLen /= ta;
        }

        var w = viewarea._width - 1;
        var h = viewarea._height - 1;

        var frame = 0.25;
        var minScreenPos = new x3dom.fields.SFVec2f(frame * w, frame * h);

        frame = 0.75;
        var maxScreenPos = new x3dom.fields.SFVec2f(frame * w, frame * h);

        var dia2 = max.subtract(min).multiply(0.5);     // half diameter
        var rw = dia2.length();                         // approx radius

        var pc = min.add(dia2);                         // center in wc
        var vc = maxScreenPos.subtract(minScreenPos).multiply(0.5);

        var rs = 1.5 * vc.length();
        vc = vc.add(minScreenPos);

        var dist = 1.0;
        if (rs > x3dom.fields.Eps) {
            dist = (rw / rs) * Math.sqrt(vc.x*vc.x + vc.y*vc.y + focalLen*focalLen);
        }

        n0 = mat.multMatrixVec(n0).normalize();
        n0 = n0.multiply(dist);
        var p0 = pc.add(n0);

        var qDir = x3dom.fields.Quaternion.rotateFromTo(new x3dom.fields.SFVec3f(0, 0, 1), n0);
        var R = qDir.toMatrix();

        var T = x3dom.fields.SFMatrix4f.translation(p0.negate());
        var M = x3dom.fields.SFMatrix4f.translation(p0);

        M = M.mult(R).mult(T).mult(M);
        var viewmat = M.inverse();

        viewarea.animateTo(viewmat, viewpoint);
    }
};


	function changeInfo(newUrl) {
		// $("#info").attr("src", "content/" + newUrl); // could be iframe
		modelName = getBlendName();
		$('#content').load("content/" + modelName + '/' + newUrl, function( response, status, xhr ) {
            if ( status == "error" ) {
                $( "#content" ).html( 'No info' );
                }
        });
	}

    function changePage() {
        $(this).is('[cont]') ? 	$('#content').load("content/" +  $(this).attr('cont')) : null;   
    }

	function resetVisibility() {

		$(this).closest('.box').find('input[type=checkbox]').each(function(index) {
		    if($(this).prop('checked') == false)
			    $(this).trigger('click');	
		});
	}


	// toggle visibility selections
	function invertVisibility() {

		$(this).closest('.box').find('input[type=checkbox]').each(function(index) {
			$(this).trigger('click');	
		});
	}


	// change camera according button name
	function changeCam() {
	
		camName = $(this).attr("cam");
		console.log(camName);
		camNode = getElementByDEF('CA_' + camName);
		if(camNode)
			camNode.setAttribute("set_bind", "true");
		
		$element.runtime.resetView();
		
		// try to find content page
		if ($(this).is('[cont]') ) {
    		changeInfo($(this).attr('cont'));
		} else {
		    changeInfo(camName + '.html');
		}

    }

 
    // set viewpoint's center of rotation to the center of an object
    function examineObject(ele) {	

        var o = x3dom.fields.SFVec3d;  
        
        // html element can be a list or select  
        if ($(ele).attr('objname') !== undefined) {
            var objName = $(ele).attr('objname');  
        } else {
            var objName = $(ele).val();
        }
        
        if($(ele).is('[cont]'))
            changeInfo($(ele).attr('cont')); 
        else
            changeInfo(objName + '.html');
            
        var axis = $(ele).attr('axis'); 
      
	    var e = $element.runtime.getActiveBindable('viewpoint');
	    console.log('examine object: '+ objName + '_TRANSFORM');
	    
        var geoNode = getElementByDEF(objName + '_TRANSFORM');
	    var shape = getShape(geoNode)
	    
	    
	    //$element.runtime.showObject(shape, axis)
	    
	    // testi alkaa
	    var tr = $element.runtime.getCurrentTransform(geoNode)
        // get real max/min points and out them array
        var min = x3dom.fields.SFVec3f.MAX();
        var max = x3dom.fields.SFVec3f.MIN();

        var vol = shape._x3domNode.getVolume();
        vol.getBounds(min, max);
        min = tr.multMatrixPnt(min);
        max = tr.multMatrixPnt(max);
        $element.runtime.showLayer(tr, min, max, axis)
	    // testi loppuu
	    
	    center = getCenterOfObject(geoNode); 
    
	    e.setAttribute('centerOfRotation', center);
	
	}

    function setSwitch() {
        var sw = $(this).attr('id');
        var node = getElementByDEF(sw);
     
        node.setAttribute('whichChoice',$(this).val())
    }

    function getBlendName() {
        var fileName = $('Inline:first').attr('URL');
        // remove '.x3d'
        var parts = fileName.split('.'); 
        return parts[0]; 
    }

    function examineLayer() {
        var tagName = $(this).prop('tagName');
        
        if (tagName == 'SELECT') 
            node = $(this).find(':selected')
        else
            node = $(this)
        
        if (node.is('[groupname]')) {
            var group = node.attr('groupname')
        } else {
            examineObject(node);
            return
        }

        if(node.is('[cont]'))
            changeInfo(node.attr('cont'));
        else
             changeInfo(group + '.html');

        var axis = 'posZ'
        if (node.is('[axis]'))
            axis = $(this).attr('axis'); 
            
        console.log('examining group: ' + group + ' with axis ' + axis)   
        // add all transformations together
        
        var centers = []; 
        blendName = getBlendName();
        $.getJSON( blendName + "_layers.json", function( data ) {
            var xCoords = [];
            var yCoords = [];
            var zCoords = [];
            var tr2 = new x3dom.fields.SFMatrix4f;
            $.each( data[group], function( key, val ) {
                // get transform and then first Shape
                if(getElementByDEF(val + '_TRANSFORM')) {
                    var node = getElementByDEF(val + '_TRANSFORM');
                    var shape = getShape(node)
                    var tr = $element.runtime.getCurrentTransform(shape)
                    var n = new x3dom.fields.SFVec3f;
                    
                    centers.push(getCenterOfObject(node)) 
                    tr2 = x3dom.fields.SFMatrix4f.copy(tr);
                    
                    // get real max/min points and out them array
                    var min = x3dom.fields.SFVec3f.MAX();
                    var max = x3dom.fields.SFVec3f.MIN();
        
                    var vol = shape._x3domNode.getVolume();
                    vol.getBounds(min, max);
                    min = tr.multMatrixPnt(min);
                    max = tr.multMatrixPnt(max);
                    
                    xCoords.push(min.x)
                    xCoords.push(max.x)
                    yCoords.push(min.y)
                    yCoords.push(max.y)
                    zCoords.push(min.z)
                    zCoords.push(max.z)
                }
                
            });

            
            var minCoords = new x3dom.fields.SFVec3f(Array.min(xCoords), Array.min(yCoords), Array.min(zCoords));
            var maxCoords = new x3dom.fields.SFVec3f(Array.max(xCoords), Array.max(yCoords), Array.max(zCoords));
            
            // calculate center of centers
            var asd = new x3dom.fields.SFVec3f;
            $.each(centers, function( index, value ){
                
                //console.log(index + ': ' +value.x);
                asd.x = asd.x + value.x;
                asd.y = asd.y + value.y;
                asd.z = asd.z + value.z;
                
            }); 
            
            xAvg = asd.x/centers.length;
            yAvg = asd.y/centers.length;
            zAvg = asd.z/centers.length;
            var avgCoords = new x3dom.fields.SFVec3f(xAvg, yAvg, zAvg);
            //console.log('avgCoords ' + avgCoords);
            
            tr2.setTranslate(avgCoords);
            //console.log('tr2:' + tr2._03)
            
            // set viewpoint and center of rotation
            $element.runtime.showLayer(tr2,minCoords,maxCoords, axis);
            setCenterRot2(tr2)
            
            //console.log('keskiarvo x:' + asd.x/centers.length);
        });
    }


    function toggleLayer() {

        var isChecked = false;		
		if ($(this).is(':checked')) {
			isChecked = true;
		}

		var trName = $(this).attr('objname') + '_TRANSFORM';
		
		if ($(this).is('[objname]')) {
		    if (getElementByDEF(trName)) {
		        var node = getElementByDEF(trName);
		        node.setAttribute("render",isChecked);
		        return
		    }
		}
		
        var group = $(this).attr('groupname');
        blendName = getBlendName();
        $.getJSON( blendName + "_layers.json", function( data ) {
            var items = [];
            $.each( data[group], function( key, val ) {
                if (getElementByDEF(val + '_TRANSFORM')) {
                    var node = getElementByDEF(val + '_TRANSFORM');
                    console.log('toggle layer object: ' + node.getAttribute("DEF"));
                    node.setAttribute("render",isChecked);
                }
            });
        });    
    }

    function toggle(node, attr) {

         if (node.attr(attr) == 'true') 
            node.attr(attr, 'false'); 
         else
            node.attr(attr, 'true');     
    }




    function getLayers() {
        
        $('#layers').empty();
        modelName = getBlendName();
        $.getJSON( modelName + "_layers.json", function( data ) {
            $.each( data, function( key, val ) {
                $('#layers').append('<li><input type="checkbox" name="'+key+'" checked>' + key + '</input></li>');
            });
        });
            	
        
		
    }

    function toggleHeadLight() {
    
         var nav = $("NavigationInfo");
         if (nav.attr('headlight') == 'true') 
            nav.attr('headlight', 'false'); 
         else
            nav.attr('headlight', 'true'); 
                        
         console.log('headlight: '+ nav.attr('headlight'));           
    }

	function toggleTextures() {
		
		var children = document.body.getElementsByTagName('ImageTexture');
		console.log(children.length);
		for (var i = 0, length = children.length; i < length; i++) {
			children[i].setAttribute("url","textures/paanu.jpg");
			console.log(children[i].getAttribute("render"));
		}
		
		var children = document.body.getElementsByTagName('TextureTransform');
		console.log(children.length);
		for (var i = 0, length = children.length; i < length; i++) {
			children[i].setAttribute("render","false");
			console.log(children[i].getAttribute("render"));
		}	
	}

// ****************************************************
//                      helpers
// ****************************************************
			
	function getElementsStartsWithId( id ) {
		var children = document.body.getElementsByTagName('*');
		var elements = [], child;
		for (var i = 0, length = children.length; i < length; i++) {
			var child = children[i];
			var def = child.getAttribute("DEF");
			if(def != null) {
				if (def.substr(0, id.length) == id)
			  		elements.push(child);
		  		}
		  	}
	  	return elements;
	}
	
	function getElementByDEF( id ) {
		var children = document.body.getElementsByTagName('*');
		var elements = [], child;
		for (var i = 0, length = children.length; i < length; i++) {
			var child = children[i];
			var def = child.getAttribute("DEF");
			if(def != null) {
				if (def == id)
			  		return child;
		  		}
		  	}
	  	return false;
	}	
	

	
    function avg(arr) {
       var sum = 0, j = 0; 
       for (var i = 0; i < arr.length, isFinite(arr[i]); i++) { 
              sum += parseFloat(arr[i]); ++j; 
        } 
       return j ? sum / j : 0; 
    };	
	
    function printProps(tr) {
        var output = '';
        for (property in tr) {
          output += property + ': ' + tr[property]+'; ';
        }
        console.log(output);    
    }



    function getTranslation(mat,tr) {
        tr.x = mat._03;
        tr.y = mat._13;
        tr.z = mat._23;

    }

    function getShape(node) {
        // USE causes troubles here. "Shape" can be elsewhere in X3D tree
        if ($(node).find('Shape').length == 0) {
            if ($(node).find('Group').length > 0) {
                g = $(node).find('Group').get(0);
                node = getElementByDEF(g.getAttribute('USE'));
                return $(node).find('Shape').get(0); 
                
            } else {
                console.log('Could not find object!');
            }
        }
        else {
            return $(node).find('Shape').get(0) 
        }
    }

    function getCenterOfObject(node) {
    
        var o = x3dom.fields.SFVec3d;
        var min = x3dom.fields.SFVec3f.MAX();
        var max = x3dom.fields.SFVec3f.MIN();
        var shape = getShape(node);

	    var transl = $element.runtime.getCurrentTransform(node);             

        var vol = shape._x3domNode.getVolume();
        vol.getBounds(min, max);
        //console.log('min: ' +min + ' max: ' + max)
        //console.log('x center ' + min.add(max))
        
       // calc center
        o = min.add(max);
        o.x = o.x/2;
        o.y = o.y/2;
        o.z = o.z/2;
        
        return transl.multMatrixPnt(o);    
    }
	
 
    function setCenterRot2(transl) {
	    var e = $element.runtime.getActiveBindable('viewpoint');
	    console.log('bind:'+ e.getAttribute("DEF"));  
	    e.setAttribute('centerOfRotation',transl._03 +' '+ transl._13  +' '+ transl._23 );  
    
    } 		 

    function resize_bg_div(){
            // This function will determine which of the three columns or the window.height
            // is the largest and then set the bg div to be that height.

            // This assumes that any div markup that is above our columns is wrapped
            // in a single div with the id=header
            var var_bg_offset = document.getElementById('header').offsetHeight;

            // First we create an array and add to it the heights of each of the three columns
            // and the window height
            array_colHeights = new Array( );
	    array_colHeights.push( document.getElementById("sideBarLeft").offsetHeight );
            array_colHeights.push( document.getElementById("centerColumn").offsetHeight );
            array_colHeights.push( document.getElementById("sideBarRight").offsetHeight );

            // Instead of the raw window.innerHeight we need to take into account the offset size
            // of our header divs
            array_colHeights.push( window.innerHeight - var_bg_offset );

            // Sorting our array in descending order
            array_colHeights.sort( function( a, b ){ return b - a; } );

            // Now we'll set our bg div to the height of our largest div or window height
            document.getElementById('bg').style.height = array_colHeights[0] + "px";
            delete array_colHeights;
            delete var_bg_offset;
    }
 
 

