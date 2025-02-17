///////////////////////////////////////////////////////////////////////////////
// Authors: blinded for review ICCAD 2020 submission 271
//          (respective ands: blinded for review ICCAD 2020 submission 271, blinded for review ICCAD 2020 submission 271, blinded for review ICCAD 2020 submission 271).
//
// BSD 3-Clause License
//
// Copyright (c) 2018, blinded for review ICCAD 2020 submission 271
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////

#include "Module.hpp"
#include <boost/geometry/geometries/adapted/c_array.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)
BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian)

void Module::setNetList(int NetId) {
  // Sets parameters given an entry in Nets file
  Netlist.push_back(NetId);
}
void Module::setParameterNodes(double width, double height) {
  // Sets parameters given an entry in Nodes file
  this -> xCoordinate = 0.0;
  this -> yCoordinate = 0.0;
  this -> width = width;
  this -> height = height;
  double points[][2] = {{0.0, 0.0}, {width, 0.0}, {width, height}, {0.0, height}};
  model::polygon< model::d2::point_xy<double> > tmp;
  append(tmp, points);
  boost::geometry::correct(tmp);
  this -> poly = tmp;
  boost::geometry::envelope(poly, envelope);
}

void Module::setParameterPl(double xCoordinate, double yCoordinate) {
  // Sets parameters given an entry in Pl file
  this -> setPos(xCoordinate, yCoordinate);
  initialX = xCoordinate;
  initialY = yCoordinate;
  this -> sigma = 50.0;
}

void Module::setPos(double x, double y) {
  // Sets position of a node object (lower-left corner)
  trans::translate_transformer<double, 2, 2> translate(x - this->xCoordinate, y - this->yCoordinate);
  model::polygon<model::d2::point_xy<double> > tmp;
  boost::geometry::transform(this->poly, tmp, translate);
  this->poly = tmp;
  updateCoordinates();
}

/**
upateCoordinates
Updates parameters of Node class from a geometry object
*/
void Module::updateCoordinates() {
  boost::geometry::model::d2::point_xy<double> centroid;
  boost::geometry::centroid(this->poly, centroid);
  this -> xBy2 = centroid.get<0>();
  this -> yBy2 = centroid.get<1>();

  boost::geometry::model::box< model::d2::point_xy<double> > envtmp;
  boost::geometry::envelope(this->poly, envtmp);
  this->envelope = envtmp;
  Point minCorner = this->envelope.min_corner();
  Point maxCorner = this->envelope.max_corner();
  this->width  = maxCorner.get<0>() - minCorner.get<0>();
  this->height = maxCorner.get<1>() - minCorner.get<1>();
  this->xCoordinate = bg::get<bg::min_corner, 0>(this->envelope);
  this->yCoordinate = bg::get<bg::min_corner, 1>(this->envelope);
}

/**
printExterior
print polygon vertices
*/
void Module::printExterior() const{
  for(auto it = boost::begin(boost::geometry::exterior_ring(this->poly)); it != boost::end(boost::geometry::exterior_ring(this->poly)); ++it) {
      double x = bg::get<0>(*it);
      double y = bg::get<1>(*it);
      cout << x << " " << y << endl;
  }
}

/**
printParameter
print node params
*/
void Module::printParameter() {
  cout << "name      " << this -> name << endl;
  cout << "Width         " << this -> width << endl;
  cout << "Height        " << this -> height << endl;
  cout << "X_Co-ordinate " << this -> xCoordinate << endl;
  cout << "Y_Co-ordinate " << this -> yCoordinate << endl;
  cout << "X/2           " << xBy2 << endl;
  cout << "Y/2           " << yBy2 << endl;
  cout << "NetList       ";
  vector < int > ::iterator it2;
  for (it2 = Netlist.begin(); it2 != Netlist.end(); ++it2) {
    cout << * it2 << " ";
  }
  this -> printExterior();
  cout << "\n" << endl;
}

void Module::init_module(int id, int lev, bool r) {
  root = r;
  idx = id;
  level = lev;
}
void Module::add_child(Module* m) {
  children.push_back(m);
}
void Module::insert_cell(int i) {
  cells.push_back(i);
}
bool Module::isleaf() {
  return leaf;
}
/*
void Hierarchy::init_hierarchy(int nl, vector <int> nm) {
  num_levels = nl;
  num_modules_per_layer = nm;

  for (int l=0; l<=num_levels; ++l) {
    Level tmp;
    tmp.level = l;
    levels.push_back(tmp);
  }
  root = create_hierarchy(0,0,true);
  root->root = true;
}

Module* Hierarchy::create_hierarchy(int id, int lev, bool r) {
  Module *m = new Module;
  m->init_module(id, lev, r);
  levels[lev].modules.push_back(m);

  if (num_levels <= lev) {
    m->leaf = true;
    return m;
  }

  int num_children = num_modules_per_layer[lev];
  for (int i =1; i<=num_children; ++i) {
    Module* mc;
    mc = create_hierarchy(i, lev+1, false);
    mc->parent = m;
    if(mc) {
      m->add_child(mc);
    }
  }
  return m;
}

void Hierarchy::print_param(int i) {
  int ii = 0;
  for (auto &lvl : levels) {
    if (i >= 0 && i == ii) { 
      for (auto &m : lvl.modules) {
        cout << m->idx << " " << m->width << " " << m->height << endl;
      }
    }
    ii++ ;
  } 
}

void Hierarchy::insert_cell(int cell_id, vector < int> cluster_id_vec, Module *m, int level) {
  if(num_levels <= level) {
    return;
  }
  if(cluster_id_vec.size() < 1) {
    // macro cell - just insert into every level
    vector < Module * > macroModulePath;
    for(int l=0; l<=num_levels; ++l) {
      Module *tmp = new Module;
      tmp->init_module(levels[l].modules.size() + 1, l, false);
      tmp->macroModule = true;
      tmp->fixed = 1;
      tmp->terminal = 1;
      tmp->insert_cell(cell_id);
      levels[l].modules.push_back(tmp);
      macroModulePath.push_back(tmp);
    }
    for(int l=1; l<num_levels; ++l) {
      Module *tmp = macroModulePath[l-1];
      if (l == 1) {
        root->children.push_back(tmp);
        tmp->parent = root;
      } else {
        tmp->parent = macroModulePath[l-2];
      }
      tmp->children.push_back(macroModulePath[l]);
    }
    macroModulePath[num_levels-1]->parent = macroModulePath[num_levels-2];
    macroModulePath[num_levels-1]->children.push_back(macroModulePath[num_levels]);
    macroModulePath[num_levels]->parent = macroModulePath[num_levels-1];
    macroModulePath[num_levels]->leaf = true;
  } else {
    int cluster_id = cluster_id_vec[level];
    m->children[cluster_id-1]->insert_cell(cell_id);
    insert_cell(cell_id, cluster_id_vec, m->children[cluster_id-1], level+1);
  }
}

Module *Hierarchy::get_leaf_module_from_id(int idx, Module *m) {
  if(m->leaf) {
    return m;
  } else {
    for (auto &child : m->children) {
      if(std::find(child->cells.begin(), child->cells.end(), idx) != child->cells.end()) {
        return get_leaf_module_from_id(idx, child);
      }
    }
  }
}

  Module *Hierarchy::get_level_module_from_id(int idx, int level, Module *m) {
  if(m->level == level) {
    return m;
  } else {
    for (auto &child : m->children) {
      if(std::find(child->cells.begin(), child->cells.end(), idx) != child->cells.end()) {
        return get_level_module_from_id(idx,level, child);
      }
    }
  }
}
*/
/**
for each level in the hierarchy, instantiate a netlist for that level:
associated nets for each module (list of net ids)
netlist-module mapping id -> list of modules
*/
/*void Hierarchy::set_netlist_hierarchy(map<int, vector<pPin> > netToCell) {
  int netidx = 1;
  for (auto &net : netToCell) { 
    int netid = net.first;
    vector<pPin> pvec = net.second; //pvec_i.idx -> cell id
    vector<Module *> ms; // convert vector of pins to vector of clusters for that net add to netToModule in leaf-level
    for (auto &pin : pvec) {
      int cell_id = pin.idx;
      Module *m = get_leaf_module_from_id(cell_id, root);
      if(!m->leaf || !(std::find(m->cells.begin(), m->cells.end(), cell_id) != m->cells.end())) {
        cout << "[ERR] set_netlist_hierarchy: cell not found in leaf nodes: " << cell_id << " " << m->idx << " " << m->leaf << endl;
        return;
      }

      if(!(std::find(ms.begin(), ms.end(), m) != ms.end())) { // uniqueify module-level netlist
        m->setNetList(netidx);
        ms.push_back(m);
      }

    }
    levels.back().netToModule.insert(pair < int, vector < Module * > > (netidx, ms));
    netidx ++;
  }
  // now need to propagate up the hierarchy
  propagate_netlist(levels.back().level);
}

void Hierarchy::propagate_netlist(int lev) {
  if (lev <= 0) {
    return;
  }
  map<int, vector< Module * > >netToModules = levels[lev].netToModule;
  int netidx = 1;
  for (auto &net : netToModules) { 
    int netid = net.first;
    vector<Module *> pvec = net.second; //pvec_i.idx -> cell id
    vector<Module *> ms; // convert vector of pins to vector of clusters for that net add to netToModule in leaf-level

    for (auto &module : pvec) {
      Module *m = module->parent;
      if(!(std::find(ms.begin(), ms.end(), m) != ms.end())) { // uniqueify module-level netlist
        m->setNetList(netidx); // crash here, module is root for some reason so parent m is garbage
        ms.push_back(m);
      }
    }
    if (ms.size() > 0 && lev > 0) {
      levels[lev-1].netToModule.insert(pair < int, vector < Module* > > (netidx, ms));
    }
    netidx ++;
  }
  propagate_netlist(lev - 1);
}

void Hierarchy::set_module_geometries(vector < Node > nodeId) { // should fix to be bottom up
  // first get leaf params
  int dim = 0;

  double area = 0.0;
  double xcenter = 0.0;
  double ycenter = 0.0;

  for (auto &m : levels.back().modules) {
    double xmin = 99999999999999.0;
    double ymin = 99999999999999.0;
    double xmax = 0.0;
    double ymax = 0.0;

    double area = 0.0;
    double xcenter = 0.0;
    double ycenter = 0.0;

    for (auto &cellid : m->cells) {
      double cellx = nodeId[cellid].xCoordinate;
      double celly = nodeId[cellid].yCoordinate;
      double cellw = nodeId[cellid].width;
      double cellh = nodeId[cellid].height;

      xmin = min(xmin, cellx);
      ymin = min(ymin, celly);
      xmax = max(xmax, cellx + cellw);
      ymax = max(ymax, celly + cellh);

      area += cellw * cellh;
      xcenter += cellx;
      ycenter += celly;
    }

    xcenter = xcenter / m->cells.size();
    ycenter = ycenter / m->cells.size();

    //cout << m->idx << " " << xmax - xmin<< " " << ymax - ymin << endl;
    //m->setParameterNodes(xmax - xmin, ymax - ymin);
    //m->setParameterPl(xmin, ymin);
    dim = ceil(sqrt(area));
    m->setParameterNodes(dim, dim);
    m->setParameterPl(xcenter, ycenter);
  }
  propagate_geometries(num_levels-1);
}

void Hierarchy::propagate_geometries(int lev) {
  if (lev <= 0) {
    return;
  }

  int dim = 0;
  vector <Module *> modules = levels[lev].modules;
  for (auto &module : modules) {
    double xmin = 99999999999999.0;
    double ymin = 99999999999999.0;
    double xmax = 0.0;
    double ymax = 0.0;

    double area = 0.0;
    double xcenter = 0.0;
    double ycenter = 0.0;

    for (auto &m : module->children) {
      double cellx = m->xCoordinate;
      double celly = m->yCoordinate;
      double cellw = m->width;
      double cellh = m->height;

      xmin = min(xmin, cellx);
      ymin = min(ymin, celly);
      xmax = max(xmax, cellx + cellw);
      ymax = max(ymax, celly + cellh);

      area += cellw * cellh;
      xcenter += cellx;
      ycenter += celly;
    }

    xcenter = xcenter / module->children.size();
    ycenter = ycenter / module->children.size();

    //module->setParameterNodes(xmax - xmin, ymax - ymin);
    //module->setParameterPl(xmin, ymin);

    dim = ceil(sqrt(area));
    module->setParameterNodes(dim, dim);
    module->setParameterPl(xcenter, ycenter);  
  }
  propagate_geometries(lev - 1);
}

vector < Node >  Hierarchy::update_cell_positions_at_level(vector < Node > nodeId, int level) {
  for (auto &node : nodeId) {
    int cell_id = node.idx;
    Module *m = get_level_module_from_id(cell_id, level, root);
    if(!(std::find(m->cells.begin(), m->cells.end(), cell_id) != m->cells.end())) {
      cout << "[ERR] update_cell_positions_at_level: cell not found in leaf nodes: " << cell_id << " " << m->idx << " " << m->leaf << endl;
      return nodeId;
    }
    double mx = m->xCoordinate;
    double my = m->yCoordinate;
    double cx = node.initialX;
    double cy = node.initialY;
    double mx_orig = m->initialX;
    double my_orig = m->initialY;

    double trans_x = mx - mx_orig;
    double trans_y = my - my_orig;

    double new_cell_x = cx + trans_x;
    double new_cell_y = cy + trans_y;

    node.setPos(new_cell_x, new_cell_y);
  }
  return nodeId;
}

vector < Node >  Hierarchy::update_cell_positions(vector < Node > nodeId) {
  for (auto &node : nodeId) {
    int cell_id = node.idx;
    Module *m = get_leaf_module_from_id(cell_id, root);
    if(!m->leaf || !(std::find(m->cells.begin(), m->cells.end(), cell_id) != m->cells.end())) {
      cout << "[ERR] update_cell_positions: cell not found in leaf nodes: " << cell_id << " " << m->idx << " " << m->leaf << endl;
      return nodeId;
    }
    double mx = m->xCoordinate;
    double my = m->yCoordinate;
    double cx = node.initialX;
    double cy = node.initialY;
    double mx_orig = m->initialX;
    double my_orig = m->initialY;

    double trans_x = mx - mx_orig;
    double trans_y = my - my_orig;

    double new_cell_x = cx + trans_x;
    double new_cell_y = cy + trans_y;

    node.setPos(new_cell_x, new_cell_y);
  }
  return nodeId;
}*/
