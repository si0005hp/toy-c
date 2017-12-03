describe 'main' do
  before do
  end

  def run_script(commands)
    raw_output = nil
    IO.popen("./main", "r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  it 'integer' do
    result = run_script([
      "1",
      "999",
    ])
    expect(result).to eq([
      "1",
      "999",
    ])
  end
  
  it 'add' do
    result = run_script([
      "1+2",
      "1 + 2",
      "1 + 2 + 3 + 4 + 5",
    ])
    expect(result).to eq([
      "3",
      "3",
      "15",
    ])
  end
  
  it 'sub' do
    result = run_script([
      "3-1",
      "10 - 3 - 2 - 1",
      "5 + 3 - 2 + 1 - 4 + 9",
    ])
    expect(result).to eq([
      "2",
      "4",
      "12",
    ])
  end
end
