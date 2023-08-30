#include <gtest/gtest.h>
#include "modules/module_base.h"

#pragma once

namespace tesT_module_base
{

bool begin_ran = false;
bool tf_ran    = false;

class ModuleDerived : public PTS::Module<>
{
 public:
  ModuleDerived(const std::string &module_name) : Module(module_name) { }

  void begin() const override { begin_ran = true; }

  void threadFunc() const override { tf_ran = true; }
};

}

TEST(Module, name)
{
  tesT_module_base::ModuleDerived module("module_name");

  ASSERT_EQ(std::string("module_name"), module.getName());
}

TEST(Module, begin)
{
  tesT_module_base::ModuleDerived module("module_name");

  module.begin();

  ASSERT_TRUE(tesT_module_base::begin_ran);
}

TEST(Module, threadFunc)
{
  tesT_module_base::ModuleDerived module("module_name");

  module.start();

  delay(100);

  ASSERT_TRUE(tesT_module_base::begin_ran);

  module.destroy();
}