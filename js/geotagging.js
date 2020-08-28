var icao =  L.tileLayer('http://193.0.231.23/tiles/cz/icao/{z}/{x}/{y}.png', {
                            'tms': true,
                            'maxZoom': 11,
                        });
var cza = L.tileLayer('https://pcmlich.fit.vutbr.cz/map/tiles/{z}/{x}/{y}.png');


var osm = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                          attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, ' +
                                       '<a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, ' +
                                       'Imagery © <a href="http://mapbox.com">Mapbox</a>',
                          subdomains:['a', 'b', 'c'],
                      });

var google = L.tileLayer('https://{s}.google.com/vt/lyrs=m@248407269&hl=x-local&x={x}&y={y}&z={z}&s=Galileo', {
                             attribution: 'Map data &copy; Google 2012',
                             subdomains:['mt0','mt1','mt2','mt3'],
                         });

var satelite  = L.tileLayer('https://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}',{
                                maxZoom: 20,
                                subdomains:['mt0','mt1','mt2','mt3']
                            });

var prosoar = L.tileLayer('https://skylines.aero/mapproxy/tiles/1.0.0/airspace+airports/EPSG3857/{z}/{x}/{y}.png',{
                              attribution: 'skylines.aero'
                          });

var baseMaps = {
    "Openstreetmap": osm,
    "Google Roadmap" : google,
    "Google Satelite" : satelite,
    //      "ICAO": icao,
    //      "Aviation": cza,
};
var overlayMaps = {
    "Airspace" : prosoar
}

///////////////////



var map;
var markers = [];
var routes = [];
var joinedSegments = [];
var clickListener;
//var yellowMarker = "http://esa.ilmari.googlepages.com/markeryellow.png";
var yellowMarker = "./icons/marker-yellow.png";
var defaultMarker = "./icons/marker-red.png";
var elevator;

function initialize() {

    map = L.map('map', {
                        zoomAnimation: false,
                        fadeAnimation: false,
                        //            layers: [ osm, google, satelite, icao, cza ],
                        layers: [ osm ],
                    } ).setView([49.8043055, 15.4768055], 8);

    L.control.layers(baseMaps, overlayMaps).addTo(map);
    L.control.scale().addTo(map);
}

function test_add_marker() {
    addMarker(49, 16, 1, 1);
}


function setMapType(mapType) {
    switch (mapType){
    case "ROADMAP":
        map.setMapTypeId(google.maps.MapTypeId.ROADMAP);
        break;
    case "TERRAIN":
        map.setMapTypeId(google.maps.MapTypeId.TERRAIN);
        break;
    case "SATELLITE":
        map.setMapTypeId(google.maps.MapTypeId.SATELLITE);
        break;
    case "HYBRID":
        map.setMapTypeId(google.maps.MapTypeId.HYBRID);
        break;
    case "Cykloturist":
        map.setMapTypeId('CykloTurist');
        break;
    case "OSMMapnik":
        map.setMapTypeId('OSMMapnik');
        break;
    case "OSMCyklo":
        map.setMapTypeId('OSMCycleMap');
        break;
    default:
        break;
    }
}


function flipRelief(setVisible) {
    if (setVisible) {
        map.overlayMapTypes.insertAt(0, mapTuristCykloRelief);
    } else {
        map.overlayMapTypes.removeAt(0);
    }
    return true;
}

function setOldMarkerPosition(id) {
    for (var i in markers) {
        if(id === markers[i].id){
            markers[i].position = markers[i].oldPosition;
            markers[i].setMap(map);
        }
    }
}

function setNewMarkerPosition(id) {
    for (var markerIdx in markers) {
        if(id === markers[markerIdx].id){
            markers[markerIdx].oldPosition = markers[markerIdx].position;

            var newLat = markers[markerIdx].position.lat();
            var newLng = markers[markerIdx].position.lng();

            //markers[markerIdx].setMap(map);
            //return markers[markerIdx].position;

            ////////////////////////

            var ele = -1000;
            var locations = [];
            locations.push(markers[markerIdx].getPosition());

            // Create a LocationElevationRequest object using the array's one value
            var positionalRequest = {
                'locations': locations
            }
            // Initiate the location request
            elevator.getElevationForLocations(positionalRequest, function(results, status) {
                if (status == google.maps.ElevationStatus.OK) {
                    // Retrieve the first result
                    if (results[0]) {
                        ele = results[0].elevation;
                    }
                }
                //				alert(newLat+ ", " +newLng + " != " + markers[markerIdx].position.lat()+ " " + markers[markerIdx].position.lng() + " " + ele);

                mapWidget.newMarkerAdded(id, newLat, newLng, ele);
            });
        }
    }
    //return [markers[markerIdx].position.lat(),markers[markerIdx].position.lng(), ele];
}


var idList;
function settingNewMarker(iidList) {
    idList = iidList;
    map.draggableCursor = 'crosshair';
    clickListener = google.maps.event.addListener(map, 'click', function(event) {
        addNewMarkers(event.latLng);
    });

}
function endSettingNewMarker() {
    map.draggableCursor = 'auto';
    google.maps.event.removeListener(clickListener);
}


function addNewMarkers(LatLng) {

    var ele = -1000;
    var locations = [];
    locations.push(LatLng);

    // Create a LocationElevationRequest object using the array's one value
    var positionalRequest = {
        'locations': locations
    }
    
    // Initiate the location request
    elevator.getElevationForLocations(positionalRequest, function(results, status) {
        if (status === google.maps.ElevationStatus.OK) {
            // Retrieve the first result
            if (results[0]) {
                ele = results[0].elevation;
            }
        }

        for (var i = 0; i< idList.length; i=i+1)
        {
            addMarker(LatLng.lat(), LatLng.lng(), idList[i], 1);
            markerClicked(idList[i], 1);
            mapWidget.newMarkerAdded(idList[i], LatLng.lat(), LatLng.lng(), ele);

        }
    });

}

function addMarker(lat, lon, iid, isVisible) {
    var location = L.latLng(lat, lon);

    for (var i in markers) {
        if (iid === markers[i].id){
            if(lat >90 || lat > 90 || lon < -180 || lon > 180) {
                markers[i].setMap(null);
                markers.splice(i,1);
                return;
            }

            markers[i].position = location;
            markers[i].oldPosition = location;
            if(isVisible) {
                markers[i].setMap(map);
            }
            //centerInBounds(1,0);
            return;
        }
    }

    
    if(lat >90 || lat > 90 || lon < -180 || lon > 180) {
        return;
    }

    var marker = L.marker([lat, lon], {
                              draggable: true,
//                              icon: defaultMarker,
                              //title:"Muj puntik!",
                              id:iid
                          });

    if(isVisible) {
        marker.addTo(map);
    }
    marker.on('click', function() {
        mapWidget.markerClicked(marker.id)
    });
    marker.on('dragstart',function() {
        mapWidget.markerClicked(marker.id);
    });
    marker.on('dragend',function() {
        mapWidget.markerDragged(marker.id, marker.position.lat(), marker.position.lng());
    });
    markers.push(marker);

    //centerInBounds(1,0);
}
function changeRouteOpacity(id, value) {
    for (var i in routes) {
        if (id === routes[i].id)
            routes[i].setOptions({strokeOpacity: value});
    }
    for (var i in joinedSegments) {
        if (id === joinedSegments[i].id) {
            joinedSegments[i].setOptions({strokeOpacity: value});
        }
    }
}
function lineWidthChanged(id, value) {
    for (var i in routes) {
        if (id === routes[i].id) {
            routes[i].setOptions({strokeWeight: value});
        }
    }
    for (var i in joinedSegments) {
        if (id === joinedSegments[i].id) {
            joinedSegments[i].setOptions({strokeWeight: value});
        }
    }
}

function changeRouteColor(id, value) {
    for (var i in routes) {
        if(id === routes[i].id) {
            routes[i].setOptions({strokeColor: value});
        }
    }
    for (var i in joinedSegments) {
        if(id === joinedSegments[i].id) {
            joinedSegments[i].setOptions({strokeColor: value});
        }
    }
}
function setJoinSegments(setVisible) {
    for (var i in joinedSegments) {
        if(setVisible) {
            joinedSegments[i].setMap(map);
        } else {
            joinedSegments[i].setMap(null);
        }
    }
    // alert(setVisible);
}

//////////

function addRoute(routeCoordinatesList, iid, isVisible, color, isValid) {
    path = [];
    routeCoordinatesList.forEach(function(e) {
        path.push(new google.maps.LatLng(e[0], e[1]));
    });
    
    var route = new google.maps.Polyline({
                                             path: path,
                                             strokeColor: color,
                                             strokeOpacity: 1.0,
                                             strokeWeight: 3,
                                             id: iid
                                         });
    if(isVisible)
        route.setMap(map);

    var bounds = new google.maps.LatLngBounds;
    for (i in path) {
        bounds.extend(path[i]);
    }
    map.fitBounds(bounds);
    //map.panToBounds(bounds);
    if(isValid) {
        routes.push(route);
    } else {
        joinedSegments.push(route);
    }
}
function centerInBounds(fitMarkers, fitRoutes){

    if ((markers.length === 0) && (routes.length === 0))
    {
        /*var myLatlng = new google.maps.LatLng(0,0);
    var zoom = 1;
    map.setCenter(myLatLng);
    map.setZoom = zoom;*/
        return;
    }

    var bounds = L.latLngBounds();
    if (fitMarkers) {
        for (i in markers) {
            bounds.extend(markers[i].getLatLng());
        }
    }

    if (fitRoutes) {
        for (i in routes) {
            routes[i].getPath().forEach(function(e) {
                bounds.extend(e);
            });
        }
    }
    map.fitBounds(bounds);
    //map.panToBounds(bounds);
}
function markerSelected(isSelected, i, markersVisible){
    if (isSelected) {
        //map.setCenter(markers[i].position);
        map.panTo(markers[i].position);
        markers[i].setIcon(yellowMarker);
        markers[i].setZIndex(1);
        if(!markersVisible)
            markers[i].setMap(map);
    } else {
        markers[i].setIcon(defaultMarker);
        markers[i].setZIndex(0);
        if(!markersVisible)
            markers[i].setMap(null);
    }
}
function markerClicked(id, isCtrl) {
    for (var i in markers) {
        if (id === markers[i].id){
            markers[i].setIcon(yellowMarker);
            markers[i].setZIndex(1);
        } else if(!isCtrl){
            markers[i].setIcon(defaultMarker);
            markers[i].setZIndex(0);
        }
    }
}
function setMarkersVisibility(setVisible) {
    for (var i in markers) {
        if(setVisible) {
            markers[i].setMap(map);
        } else {
            if( markers[i].icon === defaultMarker) {
                markers[i].setMap(null);
            }
        }
    }
}
function setRoutesVisibility(setVisible) {
    for (var i in routes) {
        if(setVisible) {
            routes[i].setMap(map);
        } else {
            routes[i].setMap(null);
        }
    }
}

function deleteMarker(id) {
    for (var i in markers) {
        if (id === markers[i].id){
            markers[i].setMap(null);
            markers.splice(i,1);
            break;
        }
    }
}

function deleteRoute(id) {
    for(var i=routes.length - 1; i>=0; i--) {
        if (id === routes[i].id){
            routes[i].setMap(null);
            routes.splice(i,1);
        }
    }
    for(var i=joinedSegments.length - 1; i>=0; i--) {
        if (id === joinedSegments[i].id) {
            joinedSegments[i].setMap(null);
            joinedSegments.splice(i,1);
        }
    }
}
