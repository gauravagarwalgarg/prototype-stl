///////////////////////////////////////////////////////////////////////////////
// examples/automotive_ecu.cpp
//
// PrototypeSTL - Automotive ECU Safety-Critical Example
//
// Demonstrates: fixed_vector, optional, bitset, memory_arena, arena_allocator
// Scenario: Engine Control Unit managing sensor data with deterministic memory.
//
///////////////////////////////////////////////////////////////////////////////

#include "prototype/prototype.hpp"
#include <stdio.h>

namespace {

// Sensor reading with timestamp
struct SensorReading {
    uint32_t sensor_id;
    int32_t  value;       // raw ADC value
    uint32_t timestamp;   // microseconds
    uint8_t  quality;     // 0-100 quality metric
};

// Fault code
struct FaultCode {
    uint16_t code;
    uint8_t  severity; // 1=info, 2=warning, 3=critical
    uint32_t timestamp;
};

// Engine state machine
enum EngineState {
    ENGINE_OFF = 0,
    ENGINE_CRANKING = 1,
    ENGINE_RUNNING = 2,
    ENGINE_FAULT = 3
};

class EngineController {
public:
    static const size_t MAX_SENSORS = 16;
    static const size_t MAX_FAULTS = 32;
    static const size_t HISTORY_SIZE = 64;

    EngineController()
        : m_arena(m_arena_buffer, sizeof(m_arena_buffer))
        , m_state(ENGINE_OFF)
        , m_sensor_count(0)
        , m_fault_count(0)
    {
        m_status_flags.reset();
    }

    void process_sensor(const SensorReading& reading) {
        // Store latest reading
        if (reading.sensor_id < MAX_SENSORS) {
            m_latest_readings[reading.sensor_id] = reading;
            m_status_flags.set(reading.sensor_id);
        }

        // Check for fault conditions
        if (reading.quality < 10) {
            FaultCode fault;
            fault.code = 0x0100 | static_cast<uint16_t>(reading.sensor_id);
            fault.severity = 3;
            fault.timestamp = reading.timestamp;
            add_fault(fault);
        }

        // Temperature check (sensor 0 = coolant temp)
        if (reading.sensor_id == 0 && reading.value > 110) {
            FaultCode fault;
            fault.code = 0x0201;
            fault.severity = 3;
            fault.timestamp = reading.timestamp;
            add_fault(fault);
            transition_to(ENGINE_FAULT);
        }
    }

    void transition_to(EngineState new_state) {
        printf("  State transition: %d -> %d\n", (int)m_state, (int)new_state);
        m_state = new_state;
    }

    void print_status() const {
        printf("  Engine State: %d\n", (int)m_state);
        printf("  Active sensors: %zu/%zu\n", m_status_flags.count(), (size_t)MAX_SENSORS);
        printf("  Active faults: %zu\n", (size_t)m_fault_count);
        printf("  Arena used: %zu / %zu bytes\n", m_arena.used(), m_arena.capacity());
    }

private:
    void add_fault(const FaultCode& fault) {
        if (m_fault_count < MAX_FAULTS) {
            m_faults[m_fault_count++] = fault;
            printf("  FAULT: code=0x%04X severity=%d\n", fault.code, fault.severity);
        }
    }

    // All memory pre-allocated - zero heap usage
    char m_arena_buffer[4096];
    prototype::memory_arena m_arena;

    EngineState m_state;
    prototype::optional<SensorReading> m_latest_readings[MAX_SENSORS];
    prototype::bitset<MAX_SENSORS> m_status_flags;
    FaultCode m_faults[MAX_FAULTS];
    size_t m_sensor_count;
    size_t m_fault_count;
};

} // anonymous namespace

int main() {
    printf("=== Automotive ECU Example (ISO 26262 ASIL-D) ===\n\n");

    EngineController ecu;

    ecu.transition_to(ENGINE_CRANKING);
    ecu.transition_to(ENGINE_RUNNING);

    // Simulate sensor readings
    SensorReading readings[] = {
        {0, 85, 1000, 95},    // Coolant temp: 85C (normal)
        {1, 3200, 1001, 98},  // RPM: 3200
        {2, 45, 1002, 90},    // Throttle: 45%
        {3, 14, 1003, 5},     // Faulty sensor (quality=5)
        {0, 115, 2000, 95},   // Coolant temp: 115C (OVERHEAT!)
    };

    printf("\nProcessing sensor data...\n");
    for (size_t i = 0; i < sizeof(readings) / sizeof(readings[0]); ++i) {
        printf("  Sensor %u: value=%d quality=%u\n",
               readings[i].sensor_id, readings[i].value, readings[i].quality);
        ecu.process_sensor(readings[i]);
    }

    printf("\nFinal Status:\n");
    ecu.print_status();

    return 0;
}
