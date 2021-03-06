#pragma once

int GeoExtractor::realLayerFromId (const DetId &iterId){
  // Cells in the +z and -z direction will give the same layer!
  int direction = (recHitTools.getPosition(iterId).z()>0) ? 1 : -1;
  return direction * recHitTools.getLayer(iterId);
  // map layers in -z direction to a negative layer id
}


Cell *GeoExtractor::getCellPtr(const DetId &iterId)
{
  auto [detectorid, subdetid, layerid, waferortileid, cellid] = getCellHash(iterId);
  // Setup the detector
  Det &detector = detcol.detectors[detectorid];

  //Setup the subdetector
  Subdet &subdet = detector.subdetectors[subdetid];

  //Setup the layer
  Layer &layer = subdet.layers[layerid];

  Cell *cellptr;
  // setup the tiles or wafer depending on the detector
  if (isSiliconDet(detectorid))
  {
    //Setup the Wafer
    Wafer &wafer = layer.wafers[waferortileid];
    cellptr = &wafer.cells[cellid];
  }
  else
  {
    cellptr = &layer.tiles[waferortileid];
  }
  return cellptr;
}

CellHash GeoExtractor::getCellHash(const DetId &iterId)
{
  // Setup the detector
  unsigned int detectorid = iterId.det();
  Det &detector = detcol.detectors[detectorid];

  //Setup the subdetector
  unsigned int subdetid = iterId.subdetId();
  Subdet &subdet = detector.subdetectors[subdetid];

  //Setup the layer
  int layerid = realLayerFromId(iterId);
  Layer &layer = subdet.layers[layerid];

  std::pair<int, int> waferortileid;
  std::pair<int, int> cellid;
  // setup the tiles or wafer depending on the detector
  if (isSiliconDet(detectorid))
  {
    //Setup the Wafer
    waferortileid = recHitTools.getWafer(iterId);
    cellid = recHitTools.getCell(iterId);
  }
  else
  {
    HGCScintillatorDetId scid = HGCScintillatorDetId(iterId);
    waferortileid = scid.ietaphi();
    cellid = std::make_pair(0, 0);
  }
  CellHash retuple = std::make_tuple(detectorid, subdetid, layerid, waferortileid, cellid);
  return retuple;
}



// this method is needed even though we can cout << cell
// because we need to print out cells that dont exitst.
std::string GeoExtractor::printCell(unsigned int detectorid, unsigned int subdetid, int layerid, std::pair<int, int> waferid, std::pair<int, int> cellid)
{
  std::ostringstream stringStream;
  stringStream << " Det " << detectorid;
  stringStream << " Subdet " << subdetid;
  stringStream << " Layer " << layerid;
  stringStream << " Wafer (" << waferid.first << "," << waferid.second << ") ";
  stringStream << " Cell (" << cellid.first << "," << cellid.second << ")"<<"\n";
  std::string copyOfStr = stringStream.str();

  return copyOfStr;
}
