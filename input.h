#ifndef SPIN_INPUT_HEADER_H
#define SPIN_INPUT_HEADER_H
#include <vector>
#include <string>
#include <map>
#include <boost/serialization/serialization.hpp>
#include <boost/shared_array.hpp>
#include "IrrepSpace.h"
#include "SpinQuantum.h"
#include "timer.h"
#include "couplingCoeffs.h"
#include <boost/tr1/unordered_map.hpp>


namespace SpinAdapted{
class SpinBlock;

enum hamTypes {QUANTUM_CHEMISTRY, HUBBARD};
enum solveTypes {SMALL_DAVIDSON, BIG_DAVIDSON};
enum algorithmTypes {ONEDOT, TWODOT, TWODOT_TO_ONEDOT};
enum noiseTypes {RANDOM, EXCITEDSTATE};
 enum calcType {DMRG, GENBLOCK, ONEPDM, TWOPDM, FCI};

class Input {

 private:
  std::vector<int> m_thrds_per_node;
  int m_norbs;
  int m_alpha;
  int m_beta;
  int m_Sz;
  
  IrrepSpace m_total_symmetry_number;
  SpinQuantum m_molecule_quantum;
  int m_total_spin;
  int m_guess_permutations;

  std::vector<int> m_hf_occupancy;
  std::vector<double> m_weights;

  std::vector<int> m_sweep_iter_schedule;
  std::vector<int> m_sweep_state_schedule;
  std::vector<int> m_sweep_qstate_schedule;
  std::vector<double> m_sweep_tol_schedule;
  std::vector<double> m_sweep_noise_schedule;
  std::vector<double> m_sweep_additional_noise_schedule;

  bool m_do_diis;
  double m_diis_error;
  int m_start_diis_iter;
  int m_diis_keep_states;
  double m_diis_error_tol;

  calcType m_calc_type;
  noiseTypes m_noise_type;
  hamTypes m_ham_type;
  int m_nroots;
  solveTypes m_solve_type;
  bool m_do_deriv;
  bool m_do_fci;
  bool m_do_cd;
  bool m_set_Sz;
  int m_maxiter;
  double m_screen_tol;
  bool m_no_transform;
  bool m_add_noninteracting_orbs;

  int m_nquanta;
  int m_sys_add;
  int m_env_add;

  int m_deflation_min_size;
  int m_deflation_max_size;

  algorithmTypes m_algorithm_type;
  int m_twodot_to_onedot_iter;
  std::vector< std::map<SpinQuantum, int> > m_quantaToKeep;
  std::string  m_save_prefix;
  std::string m_load_prefix;
  bool m_direct;
  std::vector<double> m_orbenergies;

  int m_maxj;
  ninejCoeffs m_ninej;

  double m_sweep_tol;
  bool m_restart;
  bool m_fullrestart;
  bool m_restart_warm;
  bool m_reset_iterations;
  std::string m_oneintegral;
  std::string m_twointegral;

  std::vector<int> m_spin_vector;
  std::vector<int> m_spin_orbs_symmetry;
  int m_num_spatial_orbs;
  std::vector<int> m_spatial_to_spin;
  std::vector<int> m_spin_to_spatial;

  int m_outputlevel;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & m_thrds_per_node;
    ar & m_norbs & m_alpha & m_beta & m_solve_type & m_Sz & m_set_Sz;
    ar & m_spin_vector & m_spin_orbs_symmetry & m_guess_permutations & m_nroots & m_weights & m_hf_occupancy;
    ar & m_sweep_iter_schedule & m_sweep_state_schedule & m_sweep_qstate_schedule & m_sweep_tol_schedule & m_sweep_noise_schedule &m_sweep_additional_noise_schedule;
    ar & m_molecule_quantum & m_total_symmetry_number & m_total_spin & m_orbenergies & m_add_noninteracting_orbs;
    ar & m_save_prefix & m_load_prefix & m_direct ;
    ar & m_deflation_min_size & m_deflation_max_size & m_outputlevel;
    ar & m_algorithm_type & m_twodot_to_onedot_iter;
    ar & m_nquanta & m_sys_add & m_env_add & m_do_fci & m_no_transform & m_do_cd;
    ar & m_maxj & m_ninej & m_maxiter & m_do_deriv & m_screen_tol & m_quantaToKeep & m_noise_type;
    ar & m_sweep_tol & m_restart & m_fullrestart & m_restart_warm & m_reset_iterations & m_oneintegral & m_twointegral & m_calc_type & m_ham_type;
    ar & m_do_diis & m_diis_error & m_start_diis_iter & m_diis_keep_states & m_diis_error_tol & m_num_spatial_orbs;
    ar & m_spatial_to_spin & m_spin_to_spatial;
  }


  void initialize_defaults();

 public:
  //Input() : m_ninej(ninejCoeffs::getinstance()){}
  Input() {}
  Input (const std::string& config_name);

  cumulTimer guessgenT, multiplierT, operrotT, davidsonT, rotmatrixT, blockdavid, datatransfer;
  cumulTimer hmultiply, oneelecT, twoelecT, makeopsT, collectqT;
  cumulTimer opallocate, opcatenate, oprelease, opequateT, justmultiply;
  cumulTimer spinrotation, otherrotation, solvewf, postwfrearrange, couplingcoeff;
  cumulTimer buildsumblock, buildblockops, addnoise;
  cumulTimer s0time, s1time, s2time;

  const int& outputlevel() const {return m_outputlevel;}
  const vector<int>& spatial_to_spin() const {return m_spatial_to_spin;}
  const vector<int>& spin_to_spatial() const {return m_spin_to_spatial;}
  const double& diis_error_tol() const {return m_diis_error_tol;}
  const bool& do_diis() const {return m_do_diis;}
  const double& diis_error() const {return m_diis_error;}
  const int& start_diis_iter() const {return m_start_diis_iter;}
  const int& diis_keep_states() const {return m_diis_keep_states;}

  const bool& get_fullrestart() const {return m_fullrestart;}
  const double& get_sweep_tol() const {return m_sweep_tol;}
  const bool& get_restart() const {return m_restart;}
  const bool& get_restart_warm() const {return m_restart_warm;}
  const bool& get_reset_iterations() const {return m_reset_iterations;}
  const std::string& get_oneintegral() const {return m_oneintegral;}
  const std::string& get_twointegral() const {return m_twointegral;}
  const ninejCoeffs& get_ninej() const {return m_ninej;}
  const hamTypes &hamiltonian() const {return m_ham_type;}
  const int &guess_permutations() const { return m_guess_permutations; }
  std::vector<int> thrds_per_node() const { return m_thrds_per_node; }
  const calcType &calc_type() const { return m_calc_type; }
  const solveTypes &solve_method() const { return m_solve_type; }
  const noiseTypes &noise_type() const {return m_noise_type;}
  const bool &set_Sz() const {return m_set_Sz;}
  const algorithmTypes &algorithm_method() const { return m_algorithm_type; }
  int twodot_to_onedot_iter() const { return m_twodot_to_onedot_iter; }
  std::vector< std::map<SpinQuantum, int> >& get_quantaToKeep() { return m_quantaToKeep;}
  const std::vector<int> &hf_occupancy() const { return m_hf_occupancy; }
  const std::vector<int> &spin_orbs_symmetry() const { return m_spin_orbs_symmetry; }
  std::vector<double> weights(int sweep_iter) const;// { return m_weights; }
  std::vector<double> weights() const { return m_weights; }
  const std::vector<int> &sweep_iter_schedule() const { return m_sweep_iter_schedule; }
  const std::vector<int> &sweep_state_schedule() const { return m_sweep_state_schedule; }
  const std::vector<int> &sweep_qstate_schedule() const { return m_sweep_qstate_schedule; }
  const std::vector<double> &sweep_tol_schedule() const { return m_sweep_tol_schedule; }
  const std::vector<double> &sweep_noise_schedule() const { return m_sweep_noise_schedule; }
  const std::vector<double> &sweep_additional_noise_schedule() const { return m_sweep_additional_noise_schedule; }
  std::vector<double> &set_sweep_noise_schedule() { return m_sweep_noise_schedule; }
  std::vector<double> &set_sweep_additional_noise_schedule() { return m_sweep_additional_noise_schedule; }
  int& Sz() {return m_Sz;}
  int nroots(int sweep_iter) const;
  int nroots() const {return m_nroots;}
  int total_particle_number() const { if(!m_add_noninteracting_orbs) return (m_alpha + m_beta); else return (2*m_alpha); }
  int total_spin_number() const { if (!m_add_noninteracting_orbs) return (m_alpha - m_beta); else return 0; }
  const int &last_site() const { return m_num_spatial_orbs; }
  const bool &no_transform() const { return m_no_transform; }
  const int &deflation_min_size() const { return m_deflation_min_size; }
  const bool &direct() const { return m_direct; }
  const int &deflation_max_size() const { return m_deflation_max_size; }
  const IrrepSpace &total_symmetry_number() const { return m_total_symmetry_number; }
  const SpinQuantum &molecule_quantum() const { return m_molecule_quantum; }
  const int &sys_add() const { return m_sys_add; }
  bool add_noninteracting_orbs() const {return m_add_noninteracting_orbs;}
  const int &nquanta() const { return m_nquanta; }
  const int &env_add() const { return m_env_add; }
  const bool &do_fci() const { return m_do_fci; }
  const int &max_iter() const { return m_maxiter; }
  const double &screen_tol() const { return m_screen_tol; }
  const int &total_spin() const {return m_total_spin;}
  const std::vector<int> &spin_vector() const { return m_spin_vector; }
  const std::string &save_prefix() const { return m_save_prefix; }
  const std::string &load_prefix() const { return m_load_prefix; }
  SpinQuantum effective_molecule_quantum() {if (!m_add_noninteracting_orbs) return m_molecule_quantum; else return SpinQuantum(total_particle_number() + total_spin_number(), 0, total_symmetry_number());}  
  std::vector<double>& get_orbenergies() {return m_orbenergies;}
  int getHFQuanta(const SpinBlock& b) const;
  const bool &do_cd() const {return m_do_cd;}
  int slater_size() const {return m_norbs;}
};
}
#endif