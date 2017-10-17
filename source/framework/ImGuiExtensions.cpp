// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "ImGuiExtensions.hpp"
#include "imgui.h"

static bool constCharStringVectorGetter(
    void *vec,
    int idx,
    const char **outText
)
{
    auto *collection = static_cast<std::vector<std::string>*>(vec);
    if (collection == nullptr) { return false; }
    if (idx < 0 || idx >= collection->size()) { return false; }
    *outText = collection->at(idx).c_str();
    return true;
}

bool ImGui::InputText(
    const std::string& label,
    std::string& value,
    int maxCharacters,
    ImGuiInputTextFlags flags
)
{
   std::vector<char> data(maxCharacters);
   std::copy(std::begin(value), std::end(value), std::begin(data));

   bool returnFlag = ImGui::InputText(
       label.c_str(),
       data.data(),
       data.size(),
       flags
   );

   value = data.data();
   return returnFlag;
}

bool ImGui::Combo(
    const char* label,
    int* currIndex,
    std::vector<std::string> &values)
{
    if (values.empty()) { return false; }
    return Combo(label, currIndex, constCharStringVectorGetter,
        static_cast<void*>(&values), values.size());
}

bool ImGui::ListBox(
    const char* label,
    int* currIndex,
    std::vector<std::string> &values
)
{
    if (values.empty()) { return false; }
    return ListBox(label, currIndex, constCharStringVectorGetter,
        static_cast<void*>(&values), values.size());
}
