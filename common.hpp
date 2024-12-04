#include <LAppModel.hpp>
#include <CubismFramework.hpp>
#include <LAppPal.hpp>
#include <LAppAllocator.hpp>
#include <Log.hpp>
#include <unordered_map>
#include <mutex>
#include <MatrixManager.hpp>
#include <Default.hpp>

struct LAppModelObject
{
    LAppModel *model;
    MatrixManager matrixManager;
    size_t key;
};