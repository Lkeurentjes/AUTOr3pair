// Copyright (c) 2024. made for a proof of concept for thesis: "Automatic repair of 3D citymodels" by Lisa Keurentjes


#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include "Defenitions.h"
#include "nlohmann-json/json.hpp"
#include "DefenitionsCGAL.h"

using json = nlohmann::json;
using namespace std;

namespace AUTOr3pair
{
    class filehandler
    {
    private:
        size_t pos;
        string ReportNAME;
        string OutNAME;

        string UseCase = "Default";
        json RepairReport;

        json original;

    public:
        filehandler();


        // Input data functions
        void    set_input_file();
        void        start_report();
        static string   get_time();
        void    set_standards();
        void        set_parameters();
        bool    parse_JSON(json& j);
        bool    parse_obj(json& j);
        void        add_data_error(int error);

        // Test_repairs process functions
        void    addfeatureReport(json& report);
        void    post_processing_child_parrent(json& CO);
        void    Sort_features();
        void    update_metadata(json& j);
        void    update_feature_attributes(json& j);

        // output data functions
        void    write_report();
        void        ChangeReportBoundaries();
        void        OBJ_RingError(json& j);
        void            foundRepair(const json& repair, const string& key, int i);
        void            notfoundRepair(const json& repair);
        template <typename T>
        void        recursive_increment(T& num);
        template <typename T>
        void        get_boundary_set(T& boundary, set<int>& bset);
        void    write_output(json& j);
        void        write_CityJSON(std::ofstream& o, json& j);
        void            add_AUTOr3pair_stamp(json& j);
        void        write_OBJ(std::ofstream& o, json& j);

    };



    template <typename T>
    void filehandler::recursive_increment(T& num) {
        if (num.is_number()) {num = int(num) + 1; }
        else {
            for (int i = 0; i < num.size(); ++i) {
                recursive_increment(num[i]);
            }
        }
    }

    template <typename T>
    void filehandler::get_boundary_set(T& boundary, set<int>& bset) {
        if (boundary.is_number()) {
            bset.insert(int(boundary));
        }
        else {
            for (int i = 0; i < boundary.size(); ++i) {
                get_boundary_set(boundary[i], bset);
            }
        }
    }
} // AUTOr3pair

#endif //INPUTOUTPUT_H
