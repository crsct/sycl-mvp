/* {{{

    This file is part of gapc (GAPC - Grammars, Algebras, Products - Compiler;
      a system to compile algebraic dynamic programming programs)

    Copyright (C) 2008-2011  Georg Sauthoff
         email: gsauthof@techfak.uni-bielefeld.de or gsauthof@sdf.lonestar.org

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    * Author: gatter
    *
    * Created on June 29, 2015, 2:57 PM

}}} */


#ifndef RTLIB_ADP_SPECIALIZATION_PARETO_0_NOSORT_STEP_HH_
#define RTLIB_ADP_SPECIALIZATION_PARETO_0_NOSORT_STEP_HH_

#include <utility>

#if __cplusplus >= 201103L
#define _MOVE(__val) std::move(__val)
#define _MOVE_RANGE(__it1, __it2, __in) std::move(__it1, __it2, __in)
#else
#define _MOVE(__val) (__val)
#define _MOVE_RANGE(__it1, __it2, __in) std::copy(__it1, __it2, __in)
#endif


// append with no sort Pareto
template<class T, typename Compare>
inline void append(
  List_Ref<T> &answers, T &in, Compare &c, const bool keep_equal) {
    const int dim = c.dim;
    bool add = true;
    for (typename List_Ref<T>::iterator answer = answers.ref().begin();
         answer != answers.ref().end();) {
      bool less = false;
      bool better = false;
      for (int i = 1; i<= dim; ++i) {
          int res = c(*answer, in, i);
          switch (res) {
              case 1:
                  better = true;
                  break;
              case -1:
                  less = true;
                  break;
              default:
                  break;
          }

          if (better && less) {
              break;
          }
      }
      if (better && less) {  // no domination
          ++answer;
      } else if (better || (!better && !less && !keep_equal)) {
        // answer is always better or equal or all values equal
          add = false;
          break;
      } else if (less) {  // less && !better
          // remove from answer list
          answer = erase_element(answers, answer);
      } else {
          ++answer;
      }
    }

    if (add == true) {
      answers.ref().push_back(_MOVE(in));
    }
}


// append with no sort Pareto
template<class T, typename Compare>
inline void append(
  List_Ref<T> &answers, List_Ref<T> &inserts, Compare &c,
  const bool keep_equal) {
  // basic security tests
  if (isEmpty(inserts))
    return;
  assert(&answers.ref() != &inserts.ref());

  if (isEmpty(answers)) {
       _MOVE_RANGE(
         inserts.ref().begin(),
         inserts.ref().end(), std::back_inserter(answers.ref()));
      return;
  }

  // insert into bigger one
  if (answers.ref().size() < inserts.ref().size()) {
      std::swap(inserts, answers);
      List_Ref<T> &temp = inserts;
  }

  // do the real work
  const int dim = c.dim;

  for (typename List_Ref<T>::iterator in = inserts.ref().begin();
       in != inserts.ref().end(); in++) {
    bool add = true;
    for (typename List_Ref<T>::iterator answer = answers.ref().begin();
         answer != answers.ref().end();) {
      bool less = false;
      bool better = false;
      for (int i = 1; i<= dim; ++i) {
          int res = c(*answer, *in, i);
          switch (res) {
              case 1:
                  better = true;
                  break;
              case -1:
                  less = true;
                  break;
              default:
                  break;
          }

          if (better && less) {
              break;
          }
      }

      if (better && less) {  // no domination
          ++answer;
      } else if (better || (!better && !less && !keep_equal)) {
        // answer is always better or equal or all values equal
          add = false;
          break;
      } else if (less) {  // less && !better
          // remove from answer list
          answer = erase_element(answers, answer);
      } else {
          ++answer;
      }
    }

    if (add == true) {
      answers.ref().push_back(_MOVE(*in));
    }
  }
}



#endif  // RTLIB_ADP_SPECIALIZATION_PARETO_0_NOSORT_STEP_HH_
