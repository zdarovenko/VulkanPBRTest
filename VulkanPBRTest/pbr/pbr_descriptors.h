#pragma once

#include "pbr_device.h"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace pbr
{

    class PbrDescriptorSetLayout
    {
    public:
        class Builder
        {
        public:
            Builder(PbrDevice& pbrDevice) : pbrDevice{ pbrDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<PbrDescriptorSetLayout> build() const;

        private:
            PbrDevice& pbrDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        PbrDescriptorSetLayout(
            PbrDevice& pbrDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~PbrDescriptorSetLayout();
        PbrDescriptorSetLayout(const PbrDescriptorSetLayout&) = delete;
        PbrDescriptorSetLayout& operator=(const PbrDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        PbrDevice& pbrDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class PbrDescriptorWriter;
    };

    class PbrDescriptorPool
    {
    public:
        class Builder
        {
        public:
            Builder(PbrDevice& pbrDevice) : pbrDevice{ pbrDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<PbrDescriptorPool> build() const;

        private:
            PbrDevice& pbrDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        PbrDescriptorPool(
            PbrDevice& pbrDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~PbrDescriptorPool();
        PbrDescriptorPool(const PbrDescriptorPool&) = delete;
        PbrDescriptorPool& operator=(const PbrDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        PbrDevice& pbrDevice;
        VkDescriptorPool descriptorPool;

        friend class PbrDescriptorWriter;
    };

    class PbrDescriptorWriter
    {
    public:
        PbrDescriptorWriter(PbrDescriptorSetLayout& setLayout, PbrDescriptorPool& pool);

        PbrDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        PbrDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        PbrDescriptorSetLayout& setLayout;
        PbrDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}