# include "block_parser.h"


namespace flexMC {
    namespace {
        using enum LineType;

        void checkState(const LineParseResult &line, const detail::CheckScopeState &state, MaybeError &report) {
            if ((line.scope <= state.scope) && state.indent_required) {
                report.setError(R"_(Expected an indented block following an "IF" or "ELSE" statement)_",
                                line.line_begin.at(line.scope));
            }
            else if ((line.scope > state.scope) && !state.indent_required) {
                report.setError(R"_(Unexpected indentation; was not preceded by an "IF" or "ELSE" statement)_",
                                line.line_begin.at(line.scope));
            }
            else if ((line.line_type == else_) && (line.scope + 1 != state.scope)) {
                report.setError(R"_("ELSE" encountered that is not associated to an "IF" statement)_",
                                line.line_begin.at(line.scope));
            }
            else if (state.terminate_called_in_branch && (line.scope == state.scope)) {
                report.setError(R"_("Dead code found following "TERMINATE" statement)_",
                                line.line_begin.at(line.scope));
            }
        }

        void setState(const LineParseResult &line, detail::CheckScopeState &state) {
            if ((line.line_type == if_) || (line.line_type == else_)) {
                state.indent_required = true;
            }
            else {
                state.indent_required = false;
            }

            if ((line.line_type == terminate) && (line.scope > 0)) {
                state.terminate_called_in_branch = true;
            }
            else {
                state.terminate_called_in_branch = false;
            }
            state.scope = line.scope;
        }
    }


    std::pair<MaybeError, std::size_t> checkScopes(const std::vector<LineParseResult> &lines) {
        MaybeError report;
        detail::CheckScopeState state;
        std::size_t l_no = 1;
        for (const auto &res: lines) {
            checkState(res, state, report);
            if (report.isError()) {
                return {report, l_no};
            }
            setState(res, state);
            ++l_no;
        }
        return {report, l_no};
    }
}
