/* -*- c++ -*- ----------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */


#ifdef FIX_CLASS

FixStyle(mdi,FixMDI)

#else

#ifndef LMP_FIX_MDI_H
#define LMP_FIX_MDI_H

#include "fix.h"

namespace LAMMPS_NS {

class FixMDI : public Fix {
 public:
  FixMDI(class LAMMPS *, int, char **);
  ~FixMDI();
  int setmask();
  void init();

  char *engine_mode(int node);

  // receive and update forces
  void setup(int);
  void min_setup(int);
  void post_integrate();
  void post_force(int);
  void end_of_step();
  void min_pre_force(int); //@COORDS
  void min_post_force(int); //@FORCES

  double *add_force; // stores forces added using +FORCE command
  double potential_energy; // stores potential energy

  // current command
  char *command;

 protected:
  void exchange_forces();

 private:
  int master, ierr;
  int driver_socket;
  int most_recent_init; // which MDI init command was most recently received?
                        // 0 - none
                        // 1 - MD
                        // 2 - OPTG
  bool exit_flag;
  bool local_exit_flag;
  int current_node;
  int target_node;      // is the code supposed to advance to a particular node?
                        // 0 - none
                        // 1 - @COORDS (before pre-force calculation)
                        // 2 - @PRE-FORCES (before final force calculation)
                        // 3 - @FORCES (before time integration)
                        // -1 - after MD_INIT command
                        // -2 - after MD_INIT command followed by @PRE-FORCES

  // command to be executed at the target node
  char *target_command;

  char *id_pe;
  class Irregular *irregular;
  class Minimize *minimizer;
  class Compute *pe;
  void send_types(Error *);
  void send_masses(Error *);
  void receive_coordinates(Error *);
  void send_coordinates(Error *);
  void send_charges(Error *);
  void send_energy(Error *);
  void send_forces(Error *);
  void add_forces(Error *);
  void receive_forces(Error *);
  void send_cell(Error *);

};

}

#endif
#endif

/* ERROR/WARNING messages:

E: Illegal ... command

Self-explanatory.

E: Potential energy ID for fix mdi does not exist

Self-explanatory.

E: Cannot use MDI command without atom IDs

Self-explanatory.

E: MDI command requires consecutive atom IDs

Self-explanatory.

E: Unable to connect to driver

Self-explanatory.

E: Unable to ... driver

Self-explanatory.

E: Unknown command from driver

The driver sent a command that is not supported by the LAMMPS 
interface.  In some cases this might be because a nonsensical 
command was sent (i.e. "SCF").  In other cases, the LAMMPS 
interface might benefit from being expanded.

*/
