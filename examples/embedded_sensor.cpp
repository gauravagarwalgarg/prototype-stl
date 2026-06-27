///////////////////////////////////////////////////////////////////////////////
// examples/embedded_sensor.cpp
//
// PrototypeSTL - Embedded Sensor Node Example
//
// Demonstrates: fixed_vector, fixed_string, span, ring_buffer, pool_allocator
// Scenario: Resource-constrained MCU collecting and transmitting sensor data.
//
///////////////////////////////////////////////////////////////////////////////

#include "prototype/prototype.hpp"
#include <stdio.h>

namespace {

struct SensorSample {
    uint16_t adc_value;
    uint16_t timestamp_ms;
};

// Circular sample buffer for averaging
template <size_t N>
class SampleBuffer {
public:
    SampleBuffer() : m_count(0), m_write_idx(0) {}

    void add_sample(uint16_t value, uint16_t ts) {
        m_samples[m_write_idx].adc_value = value;
        m_samples[m_write_idx].timestamp_ms = ts;
        m_write_idx = (m_write_idx + 1) % N;
        if (m_count < N) ++m_count;
    }

    uint16_t average() const {
        if (m_count == 0) return 0;
        uint32_t sum = 0;
        for (size_t i = 0; i < m_count; ++i) {
            sum += m_samples[i].adc_value;
        }
        return static_cast<uint16_t>(sum / m_count);
    }

    size_t count() const { return m_count; }

private:
    SensorSample m_samples[N];
    size_t m_count;
    size_t m_write_idx;
};

// Packet builder for transmission
class PacketBuilder {
public:
    static const size_t MAX_PACKET = 64;

    PacketBuilder() : m_size(0) {}

    void add_byte(uint8_t b) {
        if (m_size < MAX_PACKET) m_buffer[m_size++] = b;
    }

    void add_u16(uint16_t v) {
        add_byte(static_cast<uint8_t>(v >> 8));
        add_byte(static_cast<uint8_t>(v & 0xFF));
    }

    prototype::span<uint8_t> get_packet() {
        return prototype::span<uint8_t>(m_buffer, m_size);
    }

    void reset() { m_size = 0; }

    size_t size() const { return m_size; }

private:
    uint8_t m_buffer[MAX_PACKET];
    size_t  m_size;
};

} // anonymous namespace

int main() {
    printf("=== Embedded Sensor Node Example ===\n\n");

    // Temperature sensor with 8-sample moving average
    SampleBuffer<8> temp_buffer;

    // Simulated ADC readings (10-bit, 0-1023)
    uint16_t adc_readings[] = {512, 515, 510, 520, 518, 516, 514, 519, 521, 517};
    uint16_t timestamp = 0;

    printf("Collecting samples...\n");
    for (size_t i = 0; i < sizeof(adc_readings) / sizeof(adc_readings[0]); ++i) {
        temp_buffer.add_sample(adc_readings[i], timestamp);
        timestamp += 100; // 100ms sample rate
        printf("  Sample %zu: ADC=%u, Running avg=%u\n",
               i, adc_readings[i], temp_buffer.average());
    }

    // Build transmission packet
    printf("\nBuilding TX packet...\n");
    PacketBuilder pkt;
    pkt.add_byte(0xAA); // sync byte
    pkt.add_byte(0x01); // sensor type: temperature
    pkt.add_u16(temp_buffer.average()); // averaged value
    pkt.add_u16(timestamp); // latest timestamp
    pkt.add_byte(static_cast<uint8_t>(temp_buffer.count())); // sample count

    prototype::span<uint8_t> packet = pkt.get_packet();
    printf("  Packet size: %zu bytes\n  Data: ", packet.size());
    for (size_t i = 0; i < packet.size(); ++i) {
        printf("%02X ", packet[i]);
    }
    printf("\n");

    // Demonstrate fixed_string for status message
    prototype::fixed_string<64> status;
    status = "SENSOR_OK";
    printf("\n  Status: %s (len=%zu, cap=%zu)\n",
           status.c_str(), status.size(), status.capacity());

    printf("\n  Total stack memory used: ~%zu bytes (zero heap)\n",
           sizeof(temp_buffer) + sizeof(pkt) + sizeof(status));

    return 0;
}
