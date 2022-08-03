
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

var googleTerrain = L.tileLayer('http://{s}.google.com/vt/lyrs=p&x={x}&y={y}&z={z}',{
                                    maxZoom: 20,
                                    subdomains:['mt0','mt1','mt2','mt3']
                                });

var googleHybrid = L.tileLayer('http://{s}.google.com/vt/lyrs=s,h&x={x}&y={y}&z={z}',{
                                   maxZoom: 20,
                                   subdomains:['mt0','mt1','mt2','mt3']
                               });

var satelite  = L.tileLayer('https://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}',{
                                maxZoom: 20,
                                subdomains:['mt0','mt1','mt2','mt3']
                            });

var hikebikemapv1 = L.tileLayer('http://{s}.tiles.wmflabs.org/hikebike/{z}/{x}/{y}.png', {
                                    subdomains:['a', 'b', 'c'],
                                    maxZoom: 17,
                                    attribution: 'Map Data © <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
                                });

var prosoar = L.tileLayer('https://skylines.aero/mapproxy/tiles/1.0.0/airspace+airports/EPSG3857/{z}/{x}/{y}.png',{
                              attribution: 'skylines.aero'
                          });

var hill = L.tileLayer('http://{s}.tiles.wmflabs.org/hillshading/{z}/{x}/{y}.png',{
                           maxZoom: 17,
                           attribution: 'Hillshading: SRTM3 v2 (<a href="http://www2.jpl.nasa.gov/srtm/">NASA</a>)'
                       });

var mapyCzOphoto = L.tileLayer('http://m{s}.mapserver.mapy.cz/ophoto-m/{z}-{x}-{y}',{
                                   "minZoom":5,
                                   "maxZoom":20,
                                   "subdomains": ['1','2','3','4'],
                                   "attribution":"© Seznam.cz, a.s., © EOX IT Services GmbH, © OpenStreetMap",
                               });
var mapyCzBase = L.tileLayer('http://m{s}.mapserver.mapy.cz/base-m/{z}-{x}-{y}', {
                                 "minZoom":5,
                                 "maxZoom":18,
                                 "subdomains": ['1','2','3','4'],
                                 "attribution":"© Seznam.cz, a.s., © OpenStreetMap",
                             });
var arcGisTopo = L.tileLayer('https://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/{z}/{y}/{x}.png',{
                                 "attribution":"© Esri",
                                 "subdomains":['a','b','c'],
                             });
var arcGitSatellite = L.tileLayer('https://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}.png',{
                                     "attribution":"© Esri",
                                     "subdomains":['a','b','c'],
                                     "maxZoom":17,
                                 });

var baseMaps = {
    osm,
    google,
    googleTerrain,
    googleHybrid,
    satelite,
    hikebikemapv1,
    mapyCzOphoto,
    mapyCzBase,
    arcGisTopo,
    arcGitSatellite
}

var selectedBaseLayer = osm;
var selectedOverlayLayer = null;


var overlayMaps = {
    prosoar,
    hill
}

///////////////////


var mapWidget;
var map;
var cameraMarkers = [];
var objectMarkers = [];
var fowMarkers = [];
var routes = [];
var joinedSegments = [];
var clickListener;
var marker_camera_selected;
var marker_camera_deselected;
var marker_object_deselected;
var marker_object_selected;
var elevator;
var lastSelected = -1;

function initialize() {

    new QWebChannel(qt.webChannelTransport, function (channel) {
        // now you retrieve your object
        window.mapWidget = channel.objects.mapWidget;

    });

    map = L.map('map', {
                    zoomAnimation: false,
                    fadeAnimation: false,
                    layers: [ osm ],
                } ).setView([49.8043055, 15.4768055], 8);

    L.control.scale().addTo(map);
    //    L.control.layers(baseMaps, overlayMaps).addTo(map);
    $('.leaflet-container').css('cursor','crosshair');

    marker_camera_selected = L.icon({
                              iconUrl: "qrc:///js/images/marker-icon-gold.png",
                              shadowUrl: "qrc:///js/images/marker-shadow.png",
                              iconSize: [25, 41],
                              iconAnchor: [12, 41],
                              popupAnchor: [1, -34],
                              shadowSize: [41, 41],
                          });

    marker_camera_deselected = L.icon({
                               iconUrl: "qrc:///js/images/marker-icon-blue.png",
                               shadowUrl: "qrc:///js/images/marker-shadow.png",
                               iconSize: [25, 41],
                               iconAnchor: [12, 41],
                               popupAnchor: [1, -34],
                               shadowSize: [41, 41],
                           });


    marker_object_deselected = L.icon({
                               iconUrl: "qrc:///js/images/marker-icon-red.png",
                               shadowUrl: "qrc:///js/images/marker-shadow.png",
                               iconSize: [25, 41],
                               iconAnchor: [12, 41],
                               popupAnchor: [1, -34],
                               shadowSize: [41, 41],
                           });
    marker_object_selected = L.icon({
                               iconUrl: "qrc:///js/images/marker-icon-green.png",
                               shadowUrl: "qrc:///js/images/marker-shadow.png",
                               iconSize: [25, 41],
                               iconAnchor: [12, 41],
                               popupAnchor: [1, -34],
                               shadowSize: [41, 41],
                           });

    map.on('mousemove', function (e) {
        if (lastSelected !== -1) {
            for (var i in cameraMarkers) {
                if (lastSelected === cameraMarkers[i].options.id){
                    var mouse_pos = e.latlng;
                    var marker_pos = cameraMarkers[i].getLatLng();
                    $("#distance").text( Math.round(marker_pos.distanceTo(mouse_pos)) + " m");
                    break;
                }
            }

        } else {
            $("#distance").text("");

        }
    })
//    test_add_marker();
//        test_add_route();
}

function test_add_marker() {
    addMarker(1234, true, 49.5, 16.4);
    addMarker(1235, true, 49.5, 16.5);
    addMarker(1236, true, 49.5, 16.6);
    addMarker(1237, true, 49.5, 16.7);
    addMarker(1238, true, 49.5, 16.8);

    addObjectMarker(1234, true, 49.6, 16.4);
    addObjectMarker(1235, true, 49.6, 16.5);
    addObjectMarker(1236, true, 49.6, 16.6);
    addObjectMarker(1237, true, 49.6, 16.7);
    addObjectMarker(1239, true, 49.6, 16.9);

    centerInBounds(true, true);
}


function test_add_route() {
    var route_id = 42;
    addRoute([ [49,16], [50,17], [49,17] ], route_id, true, "#ff0000", true)
    changeRouteColor(route_id, "#0000ff");
    changeRouteOpacity(route_id, 0.3);
    lineWidthChanged(route_id, 7)
}


function setMapType(mapType) {

    if (selectedOverlayLayer !== null) {
        map.removeLayer(selectedOverlayLayer);
    }

    map.removeLayer(selectedBaseLayer);

    switch (mapType){
    case "ROADMAP":
        selectedBaseLayer = google;
        break;
    case "TERRAIN":
        selectedBaseLayer = googleTerrain;
        break;
    case "SATELLITE":
        selectedBaseLayer = satelite;
        break;
    case "HYBRID":
        selectedBaseLayer = googleHybrid;
        break;
    case "Cykloturist":
        selectedBaseLayer = hikebikemapv1;
        break;
    case "OSMMapnik":
        selectedBaseLayer = osm;
        break;
    case "mapyCzBase":
        selectedBaseLayer = mapyCzBase;
        break;
    case "mapyCzOphoto":
        selectedBaseLayer = mapyCzOphoto;
        break;
    case "arcGisTopo":
        selectedBaseLayer = arcGisTopo;
        break;
    case "arcGitSatellite":
        selectedBaseLayer = arcGitSatellite;
        break;

    default:
        return false;
    }

    map.addLayer(selectedBaseLayer);
    if (selectedOverlayLayer != null) {
        map.addLayer(selectedOverlayLayer);
    }

    return true;
}

function flipRelief(setVisible) {
    if (setVisible) {
        map.addLayer(hill);
        selectedOverlayLayer = hill;
    } else {
        map.removeLayer(hill)
        selectedOverlayLayer = null;
    }
    return true;
}

function setOldMarkerPosition(id) {
    console.log("setOldMarkerPosition("+id+")")
    for (var i in cameraMarkers) {
        if (id === cameraMarkers[i].options.id) {
            var position = cameraMarkers[i].options.oldPosition;
            if (position === undefined) {
                console.error("oldPosition is not defined")
                return;
            }

            cameraMarkers[i].setLatLng(position);
            map.panTo(position)
            return;
        }
    }
}

function setOldObjectMarkerPosition(id) {
    console.log("setOldObjectMarkerPosition("+id+")")
    for (var i in objectMarkers) {
        if (id === objectMarkers[i].options.id) {
            var position = objectMarkers[i].options.oldPosition;
            if (position === undefined) {
                console.error("oldPosition is not defined");
                return;
            }
            objectMarkers[i].setLatLng(position);
            map.panTo(position)
            return;
        }
    }
}

function setNewMarkerPosition(id) {
    console.log("setNewMarkerPosition (" + id +")")
    for (var markerIdx in cameraMarkers) {
        if(id === cameraMarkers[markerIdx].options.id){
            var position = cameraMarkers[markerIdx].getLatLng();
            cameraMarkers[markerIdx].options.oldPosition = position;
            map.addLayer(cameraMarkers[markerIdx]);

            ////////////////////////

            var ele = -1000;
            window.mapWidget.newMarkerAdded(id, position.lat, position.lng, ele);
            return position;

        }
    }
    //return [cameraMarkers[markerIdx].position.lat(),cameraMarkers[markerIdx].position.lng(), ele];
}


var idList;
function settingNewMarker(iidList) {
    idList = iidList;

    $('#map').css('cursor','crosshair');

    map.once('click', function(event){
        console.log("marker Added" + event.latlng)
        addNewMarkers(event.latlng);
    });
}
function endSettingNewMarker() {
    $('#map').css('cursor','');
}


function addNewMarkers(coord) {
    console.log("FIXME addNewMarkers(" + coord + ")")
    var ele = -1000;

    // FIXME evelation
    for (var i = 0; i < idList.length; i++) {

        // addMarker(iid, isVisible, lat, lon, dir, fov, objLat, objLon)

        addMarker(idList[i], true, coord.lat, coord.lng);
        markerClicked(idList[i], 1);
        window.mapWidget.newMarkerAdded(idList[i], coord.lat, coord.lng, ele);
    }

}

function addObjectMarker(_id, _isVisible, lat, lon) {
    console.log("addObjectMarker " + _id + _isVisible, lat, lon);

    for (var i in objectMarkers) {
        if (_id === objectMarkers[i].options.id){
            if (lat < -90 || lat > 90 || lon < -180 || lon > 180) {
                map.removeLayer(objectMarkers[i]);
                objectMarkers.splice(i,1);
                return;
            }

            objectMarkers[i].setLatLng(location);
            objectMarkers[i].options.oldPosition = location;
            if (_isVisible) {
                map.addLayer(objectMarkers[i]);
            }
            //centerInBounds(1,0);
            return;
        }
    }

    if (lat < -90 || lat > 90 || lon < -180 || lon > 180) {
        console.log("Marker " + _id + " out of bounds");
        return;
    }

    var marker = L.marker([lat, lon], {
                              draggable: true,
                              icon: marker_object_deselected,
                              id: _id,
                              title: _id,
                              oldPosition: [lat, lon],
                          });

    if (_isVisible) {
        marker.addTo(map);
    }
    marker.on('click', function(e) {
        mapWidget.objectClicked(marker.options.id)
    });
//    marker.on('dragstart',function() { // 'dragstart'
//        //        markerClicked(marker.options.id, false); // FIXME: doesn't work https://github.com/Leaflet/Leaflet/issues/4484
//    });
    marker.on('dragend',function() { // 'dragend'
        //        console.log('markerDragged(' + marker.options.id+ ")");
        window.mapWidget.objectDragged(marker.options.id);
    });
    objectMarkers.push(marker);


}

function deleteObjectMarker(id) {
    for (var i in objectMarkers) {
        if (id === objectMarkers[i].options.id){
            map.removeLayer(objectMarkers[i]);
            objectMarkers.splice(i,1);
            break;
        }
    }
}


function addMarker(iid, isVisible, lat, lon) {
    console.log("addMarker("+iid+ ", "+ isVisible+ ","+lat+", "+lon+")")
    var location = L.latLng(lat, lon);

    for (var i in cameraMarkers) {
        if (iid === cameraMarkers[i].options.id){
            if (lat < -90 || lat > 90 || lon < -180 || lon > 180) {
                map.removeLayer(cameraMarkers[i]);
                cameraMarkers.splice(i,1);
                return;
            }

            cameraMarkers[i].setLatLng(location);
            cameraMarkers[i].options.oldPosition = location;
            if(isVisible) {
                map.addLayer(cameraMarkers[i]);
            }
            //centerInBounds(1,0);
            return;
        }
    }
    
    if (lat < -90 || lat > 90 || lon < -180 || lon > 180) {
        return;
    }

    var marker = L.marker([lat, lon], {
                              draggable: true,
                              icon: marker_camera_deselected,
                              id: iid,
                              title: iid,
                              oldPosition: [lat, lon],
                          });

    if (isVisible) {
        marker.addTo(map);
    }
    marker.on('click', function(e) {
        mapWidget.markerClicked(marker.options.id)
    });
    marker.on('dragstart',function() { // 'dragstart'
        //        markerClicked(marker.options.id, false); // FIXME: doesn't work https://github.com/Leaflet/Leaflet/issues/4484
    });
    marker.on('dragend',function() { // 'dragend'
        //        console.log('markerDragged(' + marker.options.id+ ")");
        window.mapWidget.markerDragged(marker.options.id);
    });
    cameraMarkers.push(marker);

    //centerInBounds(1,0);
}

/**
  * @param isSelected true/false
  * @param i index in array
  * @param markersVisible true/false
  */

function markerOrObjectSelected(id, isSelected, markersVisible) {
    console.log("markerOrObjectSelected("+id+", "+isSelected+", "+markersVisible+")")
    if (id === -1) {
        return;
    }
    lastSelected = id;

    for (var i in cameraMarkers) {
        if (cameraMarkers[i].options.id === id) {

            if (isSelected) {
                map.panTo(cameraMarkers[i].getLatLng());
                cameraMarkers[i].setIcon(marker_camera_selected);
                cameraMarkers[i].setZIndexOffset(1);
                if (!markersVisible) {
                    map.addLayer(objectMarkers[i]);
                }
            } else {
                cameraMarkers[i].setIcon(marker_camera_deselected);
                cameraMarkers[i].setZIndexOffset(0);
                if (!markersVisible) {
                    map.removeLayer(cameraMarkers[i]);
                }
            }
            break;
        }
    }

    for (var i in objectMarkers) {
        if (objectMarkers[i].options.id === id) {

            if (isSelected) {
                map.panTo(objectMarkers[i].getLatLng());
                objectMarkers[i].setIcon(marker_object_selected);
                objectMarkers[i].setZIndexOffset(1);
                if (!objectMarkers) {
                    map.addLayer(objectMarkers[i]);
                }
            } else {
                objectMarkers[i].setIcon(marker_object_deselected);
                objectMarkers[i].setZIndexOffset(0);
                if (!objectMarkers) {
                    map.removeLayer(objectMarkers[i]);
                }
            }
            break;
        }
    }

}

/**
  * changes color of selected marker and put into foreground
  * @param id of the photo
  * @param isCtrl previously selected photos will stay highlighted if true,
  *   otherwise will be other photos "deselected"
  */

function markerOrObjectClicked(id, isCtrl) {
    console.log("markerOrObjectClicked("+id+", "+isCtrl+")")
    lastSelected = id;
    for (var i in cameraMarkers) {
        if (id === cameraMarkers[i].options.id){

            cameraMarkers[i].setIcon(marker_camera_selected);
            cameraMarkers[i].setZIndexOffset(1);
        } else if(!isCtrl) {
            cameraMarkers[i].setIcon(marker_camera_deselected);
            cameraMarkers[i].setZIndexOffset(0);
        }
    }

    for (var i in objectMarkers) {
        if (id === objectMarkers[i].options.id){

            objectMarkers[i].setIcon(marker_object_selected);
            objectMarkers[i].setZIndexOffset(1);
        } else if(!isCtrl) {
            objectMarkers[i].setIcon(marker_object_deselected);
            objectMarkers[i].setZIndexOffset(0);
        }
    }

}

function setMarkersVisibility(setVisible) {
    for (var i in cameraMarkers) {
        if (setVisible) {
            map.addLayer(cameraMarkers[i]);
        } else {
//            if( cameraMarkers[i].getIcon() === marker_camera_deselected) {
                map.removeLayer(cameraMarkers[i]);
//            }
        }
    }
}

function deleteMarker(id) {
    for (var i in cameraMarkers) {
        if (id === cameraMarkers[i].options.id){
            map.removeLayer(cameraMarkers[i]);
            cameraMarkers.splice(i,1);
            break;
        }
    }
}


//////////////////// routes ////////////////////

function changeRouteOpacity(id, value) {
    for (var i in routes) {
        if (id === routes[i].options.id) {
            routes[i].setStyle({opacity: value});
        }
    }
    for (var i in joinedSegments) {
        if (id === joinedSegments[i].options.id) {
            joinedSegments[i].setStyle({opacity: value});
        }
    }
}

function lineWidthChanged(id, value) {
    for (var i in routes) {
        if (id === routes[i].options.id) {
            routes[i].setStyle({weight: value});
        }
    }
    for (var i in joinedSegments) {
        if (id === joinedSegments[i].options.id) {
            joinedSegments[i].setStyle({weight: value});
        }
    }
}

function changeRouteColor(id, value) {
    for (var i in routes) {
        if (id === routes[i].options.id) {
            routes[i].setStyle({color: value});
        }
    }
    for (var i in joinedSegments) {
        if (id === joinedSegments[i].options.id) {
            joinedSegments[i].setStyle({color: value});
        }
    }
}

function setJoinSegments(setVisible) {
    for (var i in joinedSegments) {
        if (setVisible) {
            map.addLayer(joinedSegments[i]);
        } else {
            map.removeLayer(joinedSegments[i]);
        }
    }
}

function addRoute(routeCoordinatesList, iid, isVisible, var_color, isValid) {
    console.log("addRoute(routeCoordinatesList, "+iid+", "+isVisible+", "+var_color+", "+isValid+") ")
    path = [];
    routeCoordinatesList.forEach(function(e) {
        path.push(L.latLng(e[0], e[1]));
    });
    
    var route = L.polyline(path, {
                               color: var_color,
                               id: iid,
                           })

    if(isVisible) {
        route.addTo(map);
    }

    map.fitBounds(route.getBounds());

    if (isValid) {
        routes.push(route);
    } else {
        joinedSegments.push(route);
    }
}


function setRoutesVisibility(setVisible) {
    for (var i in routes) {
        if (setVisible) {
            map.addLayer(routes[i]);
        } else {
            map.removeLayer(routes[i]);

        }
    }
}

function deleteRoute(id) {
    for (var i = routes.length - 1; i>=0; i--) {
        if (id === routes[i].options.id){
            map.removeLayer(routes[i]);
            routes.splice(i,1);
        }
    }
    for (var i = joinedSegments.length - 1; i>=0; i--) {
        if (id === joinedSegments[i].options.id) {
            map.removeLayer(joinedSegments[i]);
            joinedSegments.splice(i,1);
        }
    }
}

//////////////////// ////////////////////

function centerInBounds(fitMarkers, fitRoutes) {
    console.log("centerInBounds(fitMarkers, fitRoutes)")

    if ((cameraMarkers.length === 0) && (routes.length === 0)) {
        return;
    }

    var bounds = L.latLngBounds();
    if (fitMarkers) {
        for (i in cameraMarkers) {
            bounds.extend(cameraMarkers[i].getLatLng());
        }
        for (j in objectMarkers) {
            bounds.extend(objectMarkers[j].getLatLng());
        }
    }

    if (fitRoutes) {
        for (i in routes) {
            routes[i].getLatLngs().forEach(function(e) {
                bounds.extend(e);
            });
        }
    }
    map.fitBounds(bounds);
    //map.panToBounds(bounds);
}
